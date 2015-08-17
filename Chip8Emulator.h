#pragma once
#include <fstream>
#include <iostream>
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
class Chip8Emulator
{
	//Most systems CHIP-8 was implemented on had 4096 memory locations (all 8 bits)
	static const int MEMORYSIZE = 4096;
	unsigned char memory[MEMORYSIZE];

	//Modern C8 use 16 level stack
	static const int STACKSIZE = 16;
	unsigned short stack[STACKSIZE];

	//Keep track of where we are on the stack
	unsigned short stack_pointer;

	//Opcodes are two bytes
	unsigned short opcode;

	//16 eight bit registers
	static const int REGISTERSIZE = 16;
	unsigned char registers[REGISTERSIZE];

	//Address register
	unsigned int index_register;
	unsigned int program_counter;

	//Timers (1Hz)
	unsigned char sound_timer;
	unsigned char delay_timer;

	//2048 monochrome pixels
	static const int XGFXSIZE = 64;
	static const int YGFXSIZE = 32;
	unsigned char graphics[XGFXSIZE*YGFXSIZE];
	bool draw_flag;

	//Input pad (8 bit)
	static const int INPUTSIZE = 16;
	unsigned char input[INPUTSIZE];

	//Opcode errors
	void opcodeError();

	//control flow
	void increment_pc();
	void jump();
	void jump_offset();
	void subr_return();
	void subr_call();
	void skip_equal();
	void skip_not_equal();
	void skip_equal_reg();
	void skip_not_equal_reg();

	//Register manip
	void reg_to_reg();
	void reg_to_reg_or();
	void reg_to_reg_and();
	void reg_to_reg_xor();
	void reg_to_reg_add();
	void reg_to_reg_sub();
	void reg_to_reg_sub_inv();
	void reg_shift_rt();
	void reg_shift_lt();

	void reg_to_const();
	void reg_to_delay();
	void reg_to_rand();
	void reg_to_const_add();
	void index_to_const();
	void index_to_reg_add();
	void delay_to_reg();
	void sound_to_reg();

	void split_reg();

	void memory_from_regs();
	void regs_from_memory();

	//INPUT
	void wait_for_key();
	void skip_key_pressed();
	void skip_key_not_pressed();


	//GRAPHICS
	void index_to_reg_char();
	void clear_screen();
	void draw();

public:
	Chip8Emulator(void);
	~Chip8Emulator(void);
	
	//Loads binary file into memory
	int loadProgram(const char*);

	//Performs one CPU cycle
	void cycleCPU();

	//Gets the opcode from memory at program counter location
	void fetchOpcode();

	//Performs deciphers and performs the opcode
	int decodeOpcode();

	//Update sound and delay timers
	void updateTimers();

	//Old text graphics
	void debugGraphics();

	//Used to set input values
	void setInputDown(int index);
	void setInputUp(int index);

	bool get_draw(){return draw_flag;}
	unsigned char get_pc(){return program_counter;}
	unsigned short get_opcode(){return opcode;}
	void setDrawFlag(bool flag){draw_flag=flag;}

	//Returns graphics array
	unsigned char* getGraphics();

};

