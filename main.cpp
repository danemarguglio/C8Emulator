#include "Chip8Emulator.h"
int main()
{
	using namespace std;
	Chip8Emulator chip8emulator;
	chip8emulator.loadProgram("C8Games\\pong");//hate windows
	chip8emulator.cycleCPU();
	int x;
	cin >> x;
	return 0;
}