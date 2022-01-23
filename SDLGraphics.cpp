#include "SDLGraphics.h"


SDLGraphics::SDLGraphics(std::string binary_path)
{
	chip8emulator.loadProgram(binary_path.c_str());

	//Window Titles
	window_title_unpaused = "Chip-8 Emulator";
	window_title_paused = "Chip-8 Emulator  **PAUSED**";

	//Unpause c8 emulator
	c8_paused = false;

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
	sdl_renderer = NULL;

    //Initi SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        sdl_error = SDL_GetError();
        std::cout << "Error Init " << sdl_error << std::endl;
    }
    else
    {
        //Create window
		sdl_window = SDL_CreateWindow(window_title_unpaused, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

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
	SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
	//SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl_renderer);

	//Draw border
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
	for(int border_index = 0; border_index < 5; border_index++)
	{
		SDL_RenderFillRect(sdl_renderer, &c8_border[border_index]);
	}

	//Draw c8 pixels
	//SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for(int x = 0; x < 64; x++)
	{
		for(int y = 0; y < 32; y++)
		{
			if(graphics[(y*64)+x])
			{
				//TODO make sure we draw stuff off screen..
				SDL_RenderFillRect(sdl_renderer, &c8_graphics[x][y]);
			}

		}
	}   
	SDL_RenderPresent(sdl_renderer);
	return;
}

void SDLGraphics::pauseEmulation()
{
	if (!c8_paused)
		{
			SDL_SetWindowTitle(sdl_window,window_title_paused);
			c8_paused = true;
		}
		else
		{
			SDL_SetWindowTitle(sdl_window,window_title_unpaused);
			c8_paused = false;
		}
}

void SDLGraphics::handleInputDown()
{
	switch(sdl_event.key.keysym.sym)
	{
	//Pause emulation
	case SDLK_SPACE:
		pauseEmulation();
		break;

	//Controls
	case SDLK_1:
		chip8emulator.setInputDown(0x1);
		break;
	case SDLK_2:
		chip8emulator.setInputDown(0x2);
		break;
	case SDLK_3:
		chip8emulator.setInputDown(0x3);
		break;
	case SDLK_4:
		chip8emulator.setInputDown(0xC);
		break;

	case SDLK_q:
		chip8emulator.setInputDown(0x4);
		break;
	case SDLK_w:
		chip8emulator.setInputDown(0x5);
		break;
	case SDLK_e:
		chip8emulator.setInputDown(0x6);
		break;
	case SDLK_r:
		chip8emulator.setInputDown(0xD);
		break;

	case SDLK_a:
		chip8emulator.setInputDown(0x7);
		break;
	case SDLK_s:
		chip8emulator.setInputDown(0x8);
		break;
	case SDLK_d:
		chip8emulator.setInputDown(0x9);
		break;
	case SDLK_f:
		chip8emulator.setInputDown(0xE);
		break;

	case SDLK_z:
		chip8emulator.setInputDown(0xA);
		break;
	case SDLK_x:
		chip8emulator.setInputDown(0x0);
		break;
	case SDLK_c:
		chip8emulator.setInputDown(0xB);
		break;
	case SDLK_v:
		chip8emulator.setInputDown(0xF);
		break;

	default:
		break;
	}
}
void SDLGraphics::handleInputUp()
{
	switch(sdl_event.key.keysym.sym)
	{
	case SDLK_1:
		chip8emulator.setInputUp(0x1);
		break;
	case SDLK_2:
		chip8emulator.setInputUp(0x2);
		break;
	case SDLK_3:
		chip8emulator.setInputUp(0x3);
		break;
	case SDLK_4:
		chip8emulator.setInputUp(0xC);
		break;

	case SDLK_q:
		chip8emulator.setInputUp(0x4);
		break;
	case SDLK_w:
		chip8emulator.setInputUp(0x5);
		break;
	case SDLK_e:
		chip8emulator.setInputUp(0x6);
		break;
	case SDLK_r:
		chip8emulator.setInputUp(0xD);
		break;

	case SDLK_a:
		chip8emulator.setInputUp(0x7);
		break;
	case SDLK_s:
		chip8emulator.setInputUp(0x8);
		break;
	case SDLK_d:
		chip8emulator.setInputUp(0x9);
		break;
	case SDLK_f:
		chip8emulator.setInputUp(0xE);
		break;

	case SDLK_z:
		chip8emulator.setInputUp(0xA);
		break;
	case SDLK_x:
		chip8emulator.setInputUp(0x0);
		break;
	case SDLK_c:
		chip8emulator.setInputUp(0xB);
		break;
	case SDLK_v:
		chip8emulator.setInputUp(0xF);
		break;

	default:
		break;
	}
}


void SDLGraphics::eventLoop()
{
	while(!quit)
	{
		while( SDL_PollEvent(&sdl_event) != 0 )
		{
			if(sdl_event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(sdl_event.type == SDL_KEYUP)
			{
				handleInputUp();
			}
			else if(sdl_event.type == SDL_KEYDOWN)
			{
				handleInputDown();
			}
		}

		if(!c8_paused)
		{
			chip8emulator.cycleCPU();

			if (chip8emulator.get_draw())
			{
				graphics = chip8emulator.getGraphics();
				drawScreen();
				chip8emulator.setDrawFlag(false);
			}
		}

		SDL_Delay(1);
	}
	return;
}


SDLGraphics::~SDLGraphics(void)
{
	SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    sdl_window = NULL;
    SDL_Quit();
}
