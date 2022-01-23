#include "SDLGraphics.h"

#ifdef main
	#undef main
#endif

int main(int argc, char *argv[])
{
	SDLGraphics gfx;
	gfx.eventLoop();
	
	return 0;
}