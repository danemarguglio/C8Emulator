#pragma once

#include <SDL.h>
#include <stdio.h>

class SDLGraphics
{
	//Chip 8 is 64x32
	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;

	//SDL Window
	SDL_Window* sdl_window;
    
	//Window Surface
	SDL_Surface* window_surface;

	//Sdl events
	SDL_Event sdl_event;

	//Quit event loop
	bool quit;


	const char* sdl_error;

public:
	SDLGraphics(void);
	~SDLGraphics(void);

	void eventLoop();
};

