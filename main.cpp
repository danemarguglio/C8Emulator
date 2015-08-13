#include <iostream>
#include "Chip8Emulator.h"
int main()
{
	Chip8Emulator chip8emulator;
	chip8emulator.cycleCPU();
	std::cout <<"Hello\n";
	int x;
	std::cin >> x;
	return 0;
}