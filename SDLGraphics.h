#pragma once

#include <SDL.h>
#include <stdio.h>
#include "Chip8Emulator.h"

class SDLGraphics
{
	//Chip-8 Emulator
	Chip8Emulator chip8emulator;

	//Chip 8 is 64x32
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 320;
	const int screen_scale_x = 9;
	const int screen_scale_y = 9;

	//SDL Window
	SDL_Window* sdl_window;

	//Window Titles
	const char* window_title_unpaused;
	const char* window_title_paused;

	//SDL Renderer
	SDL_Renderer* sdl_renderer;

	//Sdl events
	SDL_Event sdl_event;

	//Quit event loop
	bool quit;

	//Pause c8 processor
	bool c8_paused;

	//copied from chip8 emulator
	unsigned char *graphics;

	//Rectangles for drawing gfx & border
	SDL_Rect c8_graphics[64][32];
	SDL_Rect c8_border[4];

	const char* sdl_error;
	void pauseEmulation();
	void drawScreen();
	void handleInputDown();
	void handleInputUp();

public:
	SDLGraphics(std::string binary_path);
	~SDLGraphics(void);
	void eventLoop();
};

