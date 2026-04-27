#include "SDLGraphics.h"
#include <cmath>


SDLGraphics::SDLGraphics(std::string binary_path)
	: sdl_window(NULL),
	  sdl_renderer(NULL),
	  sdl_audio(0),
	  beep_phase(0.0),
	  quit(false),
	  c8_paused(false),
	  graphics(NULL),
	  sdl_error(NULL)
{
	if (chip8emulator.loadProgram(binary_path.c_str()) == -1)
	{
		std::cout << "Error loading program" << std::endl;
		quit = true;
		return;
	}

	//Window Titles
	window_title_unpaused = "Chip-8 Emulator";
	window_title_paused = "Chip-8 Emulator  **PAUSED**";

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
			c8_graphics[x][y].x = x * screen_scale_x;
			c8_graphics[x][y].y = y * screen_scale_y;
			c8_graphics[x][y].w = screen_scale_x;
			c8_graphics[x][y].h = screen_scale_y;
		}
	}

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		sdl_error = SDL_GetError();
		std::cout << "Error Init " << sdl_error << std::endl;
		quit = true;
		return;
	}

	sdl_window = SDL_CreateWindow(window_title_unpaused, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(sdl_window == NULL)
	{
		sdl_error = SDL_GetError();
		std::cout << "Error CreateWindow " << sdl_error << std::endl;
		quit = true;
		return;
	}

	sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
	if(sdl_renderer == NULL)
	{
		sdl_error = SDL_GetError();
		std::cout << "Error CreateRenderer " << sdl_error << std::endl;
		quit = true;
		return;
	}
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Audio: square-wave beep generated on demand and queued.
	SDL_AudioSpec want;
	SDL_zero(want);
	want.freq = 44100;
	want.format = AUDIO_S16SYS;
	want.channels = 1;
	want.samples = 1024;
	want.callback = NULL;
	sdl_audio = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
	if(sdl_audio == 0)
	{
		// Non-fatal: run silently if audio init fails.
		std::cout << "Audio init failed: " << SDL_GetError() << std::endl;
	}
	else
	{
		SDL_PauseAudioDevice(sdl_audio, 0);
	}
}

void SDLGraphics::updateBeep(bool on)
{
	if(sdl_audio == 0)
		return;
	if(!on)
	{
		SDL_ClearQueuedAudio(sdl_audio);
		beep_phase = 0.0;
		return;
	}
	// Top up the queue so we always have ~1/60s of audio buffered.
	const int sample_rate = 44100;
	const int target_samples = sample_rate / 30;
	const int queued_bytes = SDL_GetQueuedAudioSize(sdl_audio);
	const int queued_samples = queued_bytes / sizeof(Sint16);
	if(queued_samples >= target_samples)
		return;

	const int needed = target_samples - queued_samples;
	const double tone_hz = 440.0;
	const double step = 2.0 * M_PI * tone_hz / sample_rate;
	Sint16 buf[2048];
	int written = 0;
	while(written < needed)
	{
		int chunk = needed - written;
		if(chunk > (int)(sizeof(buf)/sizeof(buf[0])))
			chunk = sizeof(buf)/sizeof(buf[0]);
		for(int i = 0; i < chunk; i++)
		{
			buf[i] = (std::sin(beep_phase) >= 0.0) ? 6000 : -6000;
			beep_phase += step;
			if(beep_phase > 2.0 * M_PI)
				beep_phase -= 2.0 * M_PI;
		}
		SDL_QueueAudio(sdl_audio, buf, chunk * sizeof(Sint16));
		written += chunk;
	}
}

void SDLGraphics::drawScreen()
{
	//Clear renderer
	SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
	//SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl_renderer);

	//Draw border
	/*
	SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
	for(int border_index = 0; border_index < 5; border_index++)
	{
		SDL_RenderFillRect(sdl_renderer, &c8_border[border_index]);
	}*/

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
	graphics = chip8emulator.getGraphics();

	// CPU runs at ~540Hz (about 9 instructions per 60Hz timer tick).
	// Timers tick at exactly 60Hz, independent of the CPU rate.
	const Uint32 timer_period_ms = 1000 / 60;
	const int cpu_per_timer_tick = 9;

	Uint32 next_timer_tick = SDL_GetTicks();

	while(!quit)
	{
		while(SDL_PollEvent(&sdl_event) != 0)
		{
			if(sdl_event.type == SDL_QUIT)
				quit = true;
			else if(sdl_event.type == SDL_KEYUP)
				handleInputUp();
			else if(sdl_event.type == SDL_KEYDOWN)
				handleInputDown();
		}

		Uint32 now = SDL_GetTicks();
		if(!c8_paused && (Sint32)(now - next_timer_tick) >= 0)
		{
			for(int i = 0; i < cpu_per_timer_tick; i++)
				chip8emulator.cycleCPU();

			chip8emulator.updateTimers();
			updateBeep(chip8emulator.get_beep());

			if(chip8emulator.get_draw())
			{
				drawScreen();
				chip8emulator.setDrawFlag(false);
			}
			next_timer_tick += timer_period_ms;
		}
		else
		{
			SDL_Delay(1);
		}
	}
}


SDLGraphics::~SDLGraphics(void)
{
	if(sdl_audio != 0)
		SDL_CloseAudioDevice(sdl_audio);
	if(sdl_renderer != NULL)
		SDL_DestroyRenderer(sdl_renderer);
	if(sdl_window != NULL)
		SDL_DestroyWindow(sdl_window);
	sdl_renderer = NULL;
	sdl_window = NULL;
	SDL_Quit();
}
