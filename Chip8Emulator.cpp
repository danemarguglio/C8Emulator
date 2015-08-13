#include "Chip8Emulator.h"


Chip8Emulator::Chip8Emulator(void)
{
	/*
		I know 0x00 and 0x000 are the same as 0.. but it helps me keep track of their sizes :(
	*/

	//Clear memory
	for(int memory_index = 0; memory_index < 4096; memory_index++)
	{
		memory[memory_index] = 0x00;
	}

	//Clear stack
	for(int stack_index = 0; stack_index < 16; stack_index++)
	{
		stack[stack_index] = 0x0000;
	}

	//Clear registers
	for(int register_index = 0; register_index < 16; register_index++)
	{
		registers[register_index] = 0x00;
	}


	//Usually applications are loaded starting at memory location 512 (0x200)
	program_counter = 0x200;

	//Clear current opcode
	opcode = 0x00;

	//Clear index register
	index_register = 0x00;

	//Clear stack pointer
	stack_pointer = 0x00;

	//Clear timers
	sound_timer = 0x00;
	delay_timer = 0x00;

	//Clear input
	for(int input_index = 0; input_index < 16; input_index++)
	{
		input[input_index] = 0x00;
	}

	//Clear display
	for(int graphics_index = 0; graphics_index< 64*32; graphics_index++)
	{
		graphics[graphics_index] = 0x00;
	}

}
Chip8Emulator::~Chip8Emulator(void)
{
}

void Chip8Emulator::loadProgram()
{
	//Load program into memory!
	return;
}

void Chip8Emulator::fetchOpcode()
{
	//Read in two bytes (2 unsigned chars) and store them in an unsigned short
	//Add 8 zeros to first byte and store the second byte behind it
	opcode = memory[program_counter] << 8 | memory[program_counter + 1];
	return;
}


void Chip8Emulator::cycleCPU()
{
	fetchOpcode();
	decodeOpcode();
	updateTimers();
	//TODO Render graphics
	return;
}

void Chip8Emulator::updateTimers()
{
	if(delay_timer > 0)
		delay_timer--;

	if(sound_timer > 0)
	{
		//Console beeps if the sound timer gets to 1..
		if(sound_timer == 1)
			int x = 0;//TODO make this into a beep!
		sound_timer--;
	}
}


//This is going to be the fun one!
int Chip8Emulator::decodeOpcode()
{

	//lets return -1 or something for invalid opcodes and halt exectuion
	return 0;
}