# Mesen with SDL2

Porting Mesen(Core) with SDL2

Copy from [libretro/Mesen](https://github.com/libretro/Mesen) commit [bb9ea02](https://github.com/libretro/Mesen/commit/bb9ea02eba28682986044a6f49329ec533aa26ba)

## Purpose

To use MesenCore as a reference for my hobby NES project.

I wanted an environment that I could freely rewrite myself.

## Build

* Install SDL2
	```
	$ brew install SDL2
	```

* Build
	```
	$ cd sdl2
	$ make -j 4
	```

* Run
	```
	$ ./mesen_with_sdl2 <ROM file path>
	```

## Key binding

|NES|Key|
|---|---|
| UP, DOWN, LEFT, RIGHT | Arrow Keys|
| Start | Enter |
| Select | Right Shift |
| A | Z |
| B | X |
