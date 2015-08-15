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
	unsigned int index_register;
	unsigned int program_counter;

	//Timers (1Hz)
	unsigned char sound_timer;
	unsigned char delay_timer;

	//2048 monochrome pixels
	unsigned char graphics[64*32];
	bool draw_flag;
	//Input pad (8 bit)
	unsigned char input[16];

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

	void cycleCPU();

	void fetchOpcode();
	int decodeOpcode();
	void updateTimers();

	int loadProgram(const char*);

	void test();

	void debugGraphics();
	void setInputDown(int index);
	void setInputUp(int index);
	bool get_draw(){
		return draw_flag;
	}
	unsigned char get_pc(){return program_counter;}
	unsigned short get_opcode(){return opcode;}
	unsigned char* getGraphics();
	void setDrawFlag(bool flag){draw_flag=flag;}

};

