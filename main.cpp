#include <Windows.h>
#include "SDLGraphics.h"

#ifdef main
	#undef main
#endif
int main(int argc, char *argv[])
{
	SDLGraphics gfx;
	gfx.eventLoop();
	/*
	using namespace std;
	Chip8Emulator chip8emulator;
	chip8emulator.loadProgram("C8Games\\tank");//hate windows
	while (1)
	{
		chip8emulator.cycleCPU();
		if (chip8emulator.get_draw()){
			chip8emulator.debugGraphics();
			Sleep(100);
			system("clear");
		}
	}
	*/
	return 0;
}