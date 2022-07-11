#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "sdl2_main.h"
#include "../Core/Console.h"
#include "../Core/PPU.h"
#include "../Core/GameDatabase.h"
#include "../Core/ControlManager.h"
#include "../Core/IAudioDevice.h"
#include "../Core/KeyManager.h"
#include "../Core/StandardController.h"
#include "../Utilities/FolderUtilities.h"

//Include game database as a byte array (representing the MesenDB.txt file)
#include "MesenDB.inc"

const int SCALE = 1;
const int WINDOW_WIDTH = 254 * SCALE;
const int WINDOW_HEIGHT = 240 * SCALE;

const Uint32 palette[] = {
	0xFF666666, 0xFF002a88, 0xFF1412a7, 0xFF3b00a4,
	0xFF5c007e, 0xFF6e0040, 0xFF6c0700, 0xFF561d00,
	0xFF333500, 0xFF0c4800, 0xFF005200, 0xFF004f08,
	0xFF00404d, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFFadadad, 0xFF155fd9, 0xFF4240ff, 0xFF7527fe,
	0xFFa01acc, 0xFFb71e7b, 0xFFb53120, 0xFF994e00,
	0xFF6b6d00, 0xFF388700, 0xFF0d9300, 0xFF008f32,
	0xFF007c8d, 0xFF000000, 0xFF000000, 0xFF000000,
	0xFFffffff, 0xFF64b0ff, 0xFF9290ff, 0xFFc676ff,
	0xFFf26aff, 0xFFff6ecc, 0xFFff8170, 0xFFea9e22,
	0xFFbcbe00, 0xFF88d800, 0xFF5ce430, 0xFF45e082,
	0xFF48cdde, 0xFF4f4f4f, 0xFF000000, 0xFF000000,
	0xFFffffff, 0xFFc0dfff, 0xFFd3d2ff, 0xFFe8c8ff,
	0xFFfac2ff, 0xFFffc4ea, 0xFFffccc5, 0xFFf7d8a5,
	0xFFe4e594, 0xFFcfef96, 0xFFbdf4ab, 0xFFb3f3cc,
	0xFFb5ebf2, 0xFFb8b8b8, 0xFF000000, 0xFF000000,
};

static std::shared_ptr<Console> _console;
static std::unique_ptr<SdlKeyManager> _keyManager;
static std::unique_ptr<SdlSoundManager> _soundManager;

uint8_t _keyState = 0;

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow(
		"Mesen with SDL2",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	if (texture == NULL) {
		printf("Could not create texture: %s\n", SDL_GetError());
		return 1;
	}

	string filePath = argv[1];
	printf("ROM: %s\n", filePath.c_str());

	FolderUtilities::SetHomeFolder(getenv("HOME"));

	_console.reset(new Console());
	_console->Init(nullptr);

	_soundManager.reset(new SdlSoundManager(_console));

	std::stringstream databaseData;
	databaseData.write((const char*)MesenDatabase, sizeof(MesenDatabase));
	GameDatabase::LoadGameDb(databaseData);

	_console->GetSettings()->SetSampleRate(44100);
	_keyManager.reset(new SdlKeyManager(_console));
	KeyManager::RegisterKeyManager(_keyManager.get());

	auto getKeyCode = [=](int port, int mask) {
		return (port << 8) | mask;
	};

	auto getKeyBindings = [=](int port) {
		KeyMappingSet keyMappings;
		keyMappings.Mapping1.A = getKeyCode(port, 0x01);
		keyMappings.Mapping1.B = getKeyCode(port, 0x02);
		keyMappings.Mapping1.Select = getKeyCode(port, 0x04);
		keyMappings.Mapping1.Start = getKeyCode(port, 0x08);
		keyMappings.Mapping1.Up = getKeyCode(port, 0x10);
		keyMappings.Mapping1.Down = getKeyCode(port, 0x20);
		keyMappings.Mapping1.Left = getKeyCode(port, 0x40);
		keyMappings.Mapping1.Right = getKeyCode(port, 0x80);
		return keyMappings;
	};

	auto fn = [&](uint32_t mask) {
		return (_keyState & mask) == mask;
	};
	_keyManager->SetGetInputState(fn);

	_console->GetSettings()->SetControllerKeys(0, getKeyBindings(0));
	_console->GetSettings()->SetControllerKeys(1, getKeyBindings(1));
	_console->GetSettings()->SetControllerKeys(2, getKeyBindings(2));
	_console->GetSettings()->SetControllerKeys(3, getKeyBindings(3));

	//Setup all "auto" ports
	RomInfo romInfo = _console->GetRomInfo();
	if(romInfo.IsInDatabase || romInfo.IsNes20Header) {
		_console->GetSettings()->InitializeInputDevices(romInfo.InputType, romInfo.System, true);
	} else {
		_console->GetSettings()->InitializeInputDevices(GameInputType::StandardControllers, GameSystem::NesNtsc, true);
	}

	_console->GetSettings()->SetMasterVolume(5.0);
	_console->GetSettings()->SetControllerType(0, ControllerType::StandardController);
	_console->GetSettings()->SetControllerType(1, ControllerType::StandardController);
	_console->GetSettings()->SetControllerType(2, ControllerType::None);
	_console->GetSettings()->SetControllerType(3, ControllerType::None);

	_console->GetSettings()->SetConsoleType(ConsoleType::Famicom);

	_console->Initialize(filePath);

	Uint32* buffer = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];
	memset(buffer, 0, WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(Uint32));

	bool quit = false;
	SDL_Event event;
	shared_ptr<StandardController> controller = std::dynamic_pointer_cast<StandardController>(_console->GetControlManager()->GetControlDevice(0));

	while (!quit) {
		Uint64 start = SDL_GetPerformanceCounter();
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_z) {
					_keyState &= !0x01;
				}
				if (event.key.keysym.sym == SDLK_x) {
					_keyState &= !0x02;
				}
				if (event.key.keysym.sym == SDLK_RSHIFT) {
					_keyState &= !0x04;
				}
				if (event.key.keysym.sym == SDLK_RETURN) {
					_keyState &= !0x08;
				}
				if (event.key.keysym.sym == SDLK_UP) {
					_keyState &= !0x10;
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					_keyState &= !0x20;
				}
				if (event.key.keysym.sym == SDLK_LEFT) {
					_keyState &= !0x40;
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					_keyState &= !0x80;
				}
				break;
			}
		}
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_Z]) {
			_keyState |= 0x01;
		}
		if (state[SDL_SCANCODE_X]) {
			_keyState |= 0x02;
		}
		if (state[SDL_SCANCODE_RSHIFT]) {
			_keyState |= 0x04;
		}
		if (state[SDL_SCANCODE_RETURN]) {
			_keyState |= 0x08;
		}
		if (state[SDL_SCANCODE_UP]) {
			_keyState |= 0x10;
		}
		if (state[SDL_SCANCODE_DOWN]) {
			_keyState |= 0x20;
		}
		if (state[SDL_SCANCODE_LEFT]) {
			_keyState |= 0x40;
		}
		if (state[SDL_SCANCODE_RIGHT]) {
			_keyState |= 0x80;
		}

		_console->RunSingleFrame();

		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			for (int x = 0; x < WINDOW_WIDTH; x++) {
				int index = ((y * WINDOW_WIDTH) + x);
				buffer[index] = palette[_console->GetPpu()->GetPixel(x, y) & 0x3F];
			}
		}

		Uint64 end = SDL_GetPerformanceCounter();

		SDL_UpdateTexture(texture, NULL, buffer, WINDOW_WIDTH * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		// Cap to 60 FPS (disable cap)
		// SDL_Delay(floor(16.666f - elapsedMS));
	}

	_console->SaveBatteries();
	_console->Release(true);
	_console.reset();

	delete[] buffer;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
