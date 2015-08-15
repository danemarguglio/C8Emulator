#include "SDLGraphics.h"


SDLGraphics::SDLGraphics(void)
{
	//Event loop quit
	quit = false;

	//SDL Window
	sdl_window = NULL;
    
	//Window Surface
	window_surface = NULL;

    //Initi SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        sdl_error = SDL_GetError();
    }
    else
    {
        //Create window
        sdl_window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

        if(sdl_window == NULL)
        {
            sdl_error = SDL_GetError();
        }
        else
        {
            window_surface = SDL_GetWindowSurface(sdl_window);
        }
    }
}

void SDLGraphics::eventLoop()
{
	while(!quit)
	{
		while( SDL_PollEvent(&sdl_event) != 0 )
		{
			if( sdl_event.type == SDL_QUIT )
				quit = true;
		}
	}
	return;
}


SDLGraphics::~SDLGraphics(void)
{
	//Deallocate surface
    SDL_FreeSurface(window_surface);

    //Destroy window
    SDL_DestroyWindow(sdl_window);
    sdl_window = NULL;

    SDL_Quit();
}
