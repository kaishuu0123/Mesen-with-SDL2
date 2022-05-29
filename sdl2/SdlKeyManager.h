#pragma once
#include "../Core/IKeyManager.h"
#include "../Core/KeyManager.h"
#include "../Core/FdsSystemActionManager.h"
#include "../Core/VsSystemActionManager.h"
#include "../Core/Console.h"

class SdlKeyManager : public IKeyManager
{
private:
	shared_ptr<Console> _console;
	std::function<bool(uint32_t)> _getInputState = nullptr;
	// retro_input_poll_t _pollInput = nullptr;
	bool _mouseButtons[3] = { false, false, false };
	bool _wasPushed[16] = { };

public:
	SdlKeyManager(shared_ptr<Console> console)
	{
		_console = console;
		KeyManager::RegisterKeyManager(this);
	}

	~SdlKeyManager()
	{
		KeyManager::RegisterKeyManager(nullptr);
	}

	void SetGetInputState(std::function<bool(uint32_t)> getInputState)
	{
		_getInputState = getInputState;
	}

	void SetPollInput(uint32_t pollInput)
	{
		// _pollInput = pollInput;
	}

	// Inherited via IKeyManager
	virtual void RefreshState() override
	{
		// if(_pollInput) {
		// 	_pollInput();
		// }

		// if(_getInputState) {
		// 	int32_t x = _getInputState(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X);
		// 	int32_t y = _getInputState(0, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y);

		// 	x += 0x8000;
		// 	y += 0x8000;

		// 	KeyManager::SetMousePosition((double)x / 0x10000, (double)y / 0x10000);

		// 	int16_t dx = _getInputState(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
		// 	int16_t dy = _getInputState(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
		// 	KeyManager::SetMouseMovement(dx, dy);

		// 	_mouseButtons[(int)MouseButton::LeftButton] = _getInputState(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT) != 0;
		// 	_mouseButtons[(int)MouseButton::RightButton] = _getInputState(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT) != 0;
		// 	_mouseButtons[(int)MouseButton::MiddleButton] = _getInputState(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE) != 0;

		// 	shared_ptr<FdsSystemActionManager> fdsSam = _console->GetSystemActionManager<FdsSystemActionManager>();
		// 	if(fdsSam) {
		// 		if(ProcessAction(RETRO_DEVICE_ID_JOYPAD_L))
		// 			fdsSam->InsertNextDisk();

		// 		if(ProcessAction(RETRO_DEVICE_ID_JOYPAD_R))
		// 			fdsSam->SwitchDiskSide();
		// 	}

		// 	shared_ptr<VsSystemActionManager> vsSam = _console->GetSystemActionManager<VsSystemActionManager>();
		// 	if(vsSam) {
		// 		if(ProcessAction(RETRO_DEVICE_ID_JOYPAD_L2))
		// 			vsSam->InsertCoin(0);
		// 		if(ProcessAction(RETRO_DEVICE_ID_JOYPAD_R2))
		// 			vsSam->InsertCoin(1);
		// 	}
		// }
	}

	virtual bool IsKeyPressed(uint32_t keyCode) override
	{
		if(keyCode > 0 && _getInputState)
			return _getInputState(keyCode) != 0;
		return false;
	}

	virtual bool IsMouseButtonPressed(MouseButton button) override
	{
		return _mouseButtons[(int)button];
	}

	virtual string GetKeyName(uint32_t keyCode) override
	{
		return string();
	}

	virtual uint32_t GetKeyCode(string keyName) override
	{
		return 0;
	}
};
