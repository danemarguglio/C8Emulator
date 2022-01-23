#include <iostream>
#include "SDLGraphics.h"

#ifdef main
	#undef main
#endif

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Usage: ./Chip8 /binary/to/load" << std::endl;
		return -1;
	}

	std::string binary_path = argv[1];

	SDLGraphics gfx = SDLGraphics(binary_path);
	gfx.eventLoop();
	
	return 0;
}