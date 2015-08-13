#pragma once
class Chip8Emulator
{
	//Most systems CHIP-8 was implemented on had 4096 memory locations (all 8 bits)
	unsigned char memory[4096];

	//Modern C8 use 16 level stack
	unsigned short stack[16];

	//Keep track of where we are on the stack
	unsigned short stack_pointer;

	//Opcodes are two bytes
	unsigned short opcode;

	//16 eight bit registers
	unsigned char registers[16];

	//Address register
	unsigned char index_register;
	unsigned char program_counter;

	//Timers (1Hz)
	unsigned char sound_timer;
	unsigned char delay_timer;

	//2048 monochrome pixels
	unsigned char graphics[64*32];

	//Input pad (8 bit)
	unsigned char input[16];

	//TODO font set
	

public:
	Chip8Emulator(void);
	~Chip8Emulator(void);

	void cycleCPU();

	void fetchOpcode();
	int decodeOpcode();
	void updateTimers();

	void loadProgram();


};

