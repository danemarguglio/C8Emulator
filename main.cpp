#include "Chip8Emulator.h"

int main(int argc, char *argv[])
{
	using namespace std;
	Chip8Emulator chip8emulator;
	chip8emulator.loadProgram("C8Games\\pong");//hate windows
	while (1)
	{
		chip8emulator.cycleCPU();
		// if (chip8emulator.get_draw())
		// 	chip8emulator.debugGraphics();
	}
	return 0;
}