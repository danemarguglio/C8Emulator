#include "SDLGraphics.h"


SDLGraphics::SDLGraphics(void)
{
	chip8emulator.loadProgram("C8Games\\pong");

	//Setup game border
	c8_border[0].x = 0; c8_border[0].y = 0; c8_border[0].w = screen_scale_x*65; c8_border[0].h = screen_scale_y; //Top
	c8_border[1].x = 0; c8_border[1].y = screen_scale_y*33; c8_border[1].w = screen_scale_x*65; c8_border[1].h = screen_scale_y; //Bottom
	c8_border[2].x = 0; c8_border[2].y = 0; c8_border[2].w = screen_scale_x; c8_border[2].h = screen_scale_y*33; //Left
	c8_border[3].x = screen_scale_x*65; c8_border[3].y = 0; c8_border[3].w = screen_scale_x; c8_border[3].h = screen_scale_y*34; //Right


	//Setup c8 pixels
	for(int x = 0; x < 64; x++)
	{
		for(int y = 0; y < 32; y++)
		{
			c8_graphics[x][y].x = (x+1)*screen_scale_x; //+1 accounts for border
			c8_graphics[x][y].y = (y+1)*screen_scale_y;
			c8_graphics[x][y].w = screen_scale_x;
			c8_graphics[x][y].h = screen_scale_x;
		}
	} 

	//Event loop quit
	quit = false;

	//Window
	sdl_window = NULL;
    
	//Renderer
	sdl_renderer;

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
			sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED );
			
			if(sdl_renderer == NULL)
				sdl_error = SDL_GetError();
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}
}

void SDLGraphics::drawScreen()
{
	//Clear renderer
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl_renderer);

	//Draw border
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
	for(int border_index = 0; border_index < 5; border_index++)
	{
		SDL_RenderFillRect(sdl_renderer, &c8_border[border_index]);
	}

	//Draw c8 pixels
	SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
	for(int x = 0; x < 64; x++)
	{
		for(int y = 0; y < 32; y++)
		{
			if(!graphics[(y*64)+x])
			{
				SDL_RenderFillRect(sdl_renderer, &c8_graphics[x][y]);
			}

		}
	}   
	SDL_RenderPresent(sdl_renderer);
	return;
}


void SDLGraphics::eventLoop()
{
	while(!quit)
	{
		int x = 0;
		while( SDL_PollEvent(&sdl_event) != 0 )
		{
			if(sdl_event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		chip8emulator.cycleCPU();

		if (chip8emulator.get_draw())
		{
			graphics = chip8emulator.getGraphics();
			drawScreen();
		}

		SDL_Delay(1);
	}

	return;
}


SDLGraphics::~SDLGraphics(void)
{
	//Destroy renderer
	SDL_DestroyRenderer(sdl_renderer);
    //Destroy window
    SDL_DestroyWindow(sdl_window);
    sdl_window = NULL;

    SDL_Quit();
}
