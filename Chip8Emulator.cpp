#include "Chip8Emulator.h"


Chip8Emulator::Chip8Emulator(void)
{
	/*
		I know 0x00 and 0x000 are the same as 0.. but it helps me keep track of their sizes :(
	*/

	//Clear memory
	for(int memory_index = 0; memory_index < 4096; memory_index++)
	{
		memory[memory_index] = 0;
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

int Chip8Emulator::loadProgram(const char* file_name)
{
	//Load program into memory!
	using namespace std;
	
	ifstream open_file(file_name, ios::in|ios::binary|ios::ate);
	
	if (open_file.is_open())
	{
		cout << "Opened file " << file_name << endl;
		streampos file_size = open_file.tellg();
		char * file_in_memory = new char [file_size];
		//Go to beginning of file
		open_file.seekg (0, ios::beg);
		//Store file in memory
		open_file.read (file_in_memory, file_size);
		open_file.close();

		cout << "File Size: " << file_size << endl;
		//TODO check if file meets our critera for openin!

		//Put file in Chip-8 memory starting at 0x200
		memcpy ( &memory[0]+0x200 , &file_in_memory, sizeof(file_in_memory));
		delete[] file_in_memory;
		return 0;
	}
	else
	{
		cout <<"FNF\n";
		return -1;
	}
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
//Report opcode errors
void Chip8Emulator::opcodeError()
{
	return;
}

//This is going to be the fun one!
int Chip8Emulator::decodeOpcode()
{
	// ABCD DEFG HIJK LMNO   opcode
	// 1111 0000 0000 0000   0xF000
	// ABCD 0000 0000 0000   &

	//Check first nibble :)
	switch(opcode & 0xF000)
	{
	case 0x0000://First byte 0x00
		switch (opcode & 0x00FF)
		{
		case 0x00E0://0x00E0 Clear screen
			break;
		case 0x00EE://0x00EE Return from subroutine
			break;
		default:
			opcodeError();
		}
		break;

	case 0x1000://0x1NNN Jump to address NNN.
		break;
	case 0x2000://0x2NNN Call subroutine at NNN.
		break;
	case 0x3000://0x3XNN Skips the next instruction if VX equals NN.
		break;
	case 0x4000://0x4XNN Skips the next instruction if VX doesn't equal NN.
		break;
	case 0x5000://0x5XY0 Skips the next instruction if VX equals VY.
		break;
	case 0x6000://0x6XNN Sets VX to NN.
		break;
	case 0x7000://0x7XNN Adds NN to VX.
		break;

	case 0x8000://0x8
		//9 opcodes here determined by last nibble
		switch(opcode & 0x000F)
		{
		case 0x0000://0x8XY0 Sets VX to the value of VY.
			break;
		case 0x0001://0x8XY1 Sets VX to VX or VY.
			break;
		case 0x0002://0x8XY2 Sets VX to VX and VY.
			break;
		case 0x0003://0x8XY3 Sets VX to VX xor VY.
			break;
		case 0x0004://0x8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
			break;
		case 0x0005://0x8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			break;
		case 0x0006://0x8XY6 Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
			break;
		case 0x0007://0x8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
			break;
		case 0x000E://0x8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
			break;
		default:
			opcodeError();
			break;
		}
		break;

	case 0x9000://0x9
		switch(opcode & 0x000F)
		{
		case 0x0000://0x9XY0 Skips the next instruction if VX doesn't equal VY.
			break;
		default:
			opcodeError();
			break;
		}
		break;

	case 0xA000://0xANNN Sets I to the address NNN.
		break;
	case 0xB000://0xBNNN Jumps to the address NNN plus V0.
		break;
	case 0xC000://0xCXNN Sets VX to the result of a bitwise and operation on a random number and NN.
		break;
	case 0xD000://0xDXYN Sprites stored in memory at location in index register (I), 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it toggles the screen pixels). Sprites are drawn starting at position VX, VY. N is the number of 8bit rows that need to be drawn. If N is greater than 1, second line continues at position VX, VY+1, and so on.
		break;

	case 0xE000://0xE

		break;

	case 0xF000://0xF
		break;


	default:
		opcodeError();
		break;
	}



	//lets return -1 or something for invalid opcodes and halt exectuion
	return 0;
}