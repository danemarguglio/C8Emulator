#include "Chip8Emulator.h"
// macro: num = 0xABCD
//        nibble(0,num) -> D
//        nibble(1,num) -> C
//        nibble(2,num) -> B
//        nibble(3,num) -> A
#define nibble(index,num) ((num & (0x000F << 4*index)) >> (4*index))


void Chip8Emulator::test(){
    using namespace std;
    registers[0] = 0xFF;
    registers[1] = 0xFF;

    opcode = 0x8014;

    reg_to_reg_add();

    cout << (int)registers[0] << " " << (int)registers[1] << " " << (int)((registers[15] << 8) + registers[0]) << endl;
}

Chip8Emulator::Chip8Emulator(void)
{
    /*
        I know 0x00 and 0x000 are the same as 0.. but it helps me keep track of their sizes :(
    */
    //We need random numbers
    srand (time(NULL));

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
    draw_flag = false;

    //Font set (i stole this )
    unsigned char fontset[80] =
    { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };
    for (int i = 0; i < 80; i++)
    {
        memory[i] = fontset[i];
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
    int size;
    
    if (open_file.is_open())
    {
        cout << "Opened file " << file_name << endl;
        streampos file_size = open_file.tellg();
        size = open_file.tellg();
        char * file_in_memory = new char [file_size];
        //Go to beginning of file
        open_file.seekg (0, ios::beg);
        //Store file in memory
        open_file.read (file_in_memory, file_size);
        open_file.close();

        cout << "File Size: " << file_size << endl;
        //TODO check if file meets our critera for openin!

        //Put file in Chip-8 memory starting at 0x200
        memcpy ( &(memory[0x200]) , file_in_memory, size);
        delete[] file_in_memory;
        for(int i=0;i<size;i+=2)
            printf("%x\n", (memory[0x200+i] << 8) | memory[0x200 + i + 1]);
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
    opcode = (memory[program_counter] << 8) | (memory[program_counter + 1]);
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



// VX = VY
void Chip8Emulator::reg_to_reg(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
	registers[x] = registers[y];
}

// VX = VY or VX
void Chip8Emulator::reg_to_reg_or(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    registers[x] = registers[y] | registers[x];
}

// VX = VY or VX
void Chip8Emulator::reg_to_reg_and(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    registers[x] = registers[y] & registers[x];
}

// VX = VY xor VX
void Chip8Emulator::reg_to_reg_xor(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    registers[x] = registers[y] ^ registers[x];
}

// VX = VY + VX
// VF = overflow ? 1 : 0
void Chip8Emulator::reg_to_reg_add(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    unsigned temp = registers[x] + registers[y];
    registers[0xF] = (temp & 0xFF00) ? 1 : 0;
    registers[x] = temp;
}

// VX = VX - VY
// VF = borrow ? 1 : 0
void Chip8Emulator::reg_to_reg_sub(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    registers[0xF] = (registers[y] > registers[x]) ? 1 : 0;
    registers[x] = registers[x] - registers[y];
}

// VX = VY - VX
// VF = borrow ? 0 : 1
void Chip8Emulator::reg_to_reg_sub_inv(){
    unsigned char x = nibble(2,opcode);
    unsigned char y = nibble(1,opcode);
    registers[0xF] = (registers[y] < registers[x]) ? 0 : 1;
    registers[x] = registers[y] - registers[x];
}

// VX = VX >> 1
// VF = right of VX
void Chip8Emulator::reg_shift_rt(){
    unsigned char x = nibble(2,opcode);
    registers[0xf] = (registers[x] & 0x1);
    registers[x] = registers[x] >> 1;
}

// VX = VX << 1
// VF = left of VX
void Chip8Emulator::reg_shift_lt(){
    unsigned char x = nibble(2,opcode);
    registers[0xf] = (registers[x] & 0x80);
    registers[x] = registers[x] << 1;
}

//Program_couter++
void Chip8Emulator::increment_pc(){
    program_counter += 2;
}

// Jump unconditionally to addr
void Chip8Emulator::jump(){
    program_counter = opcode & 0x0fff;
}

//This is going to be the fun one!
int Chip8Emulator::decodeOpcode()
{
	// ABCD DEFG HIJK LMNO   opcode
	// 1111 0000 0000 0000   0xF000
	// ABCD 0000 0000 0000   &


    increment_pc();

	//Check first nibble :)
	switch(opcode & 0xF000)
	{
	case 0x0000://First byte 0x00
		switch (opcode & 0x00FF)
		{
		case 0x00E0://0x00E0 Clear screen
			for(int graphics_index = 0; graphics_index < 64*32; graphics_index++)
			{
				graphics[graphics_index] = 0x0;
			}
			draw_flag = true;
			break;

		case 0x00EE://0x00EE Return from subroutine
			stack_pointer--; //Dec stack pointer
			program_counter = stack[stack_pointer]; //Set program counter to the value we pushed on the stack from call subroutine
			break;
		default:
			opcodeError();
		}
		break;

	case 0x1000://0x1NNN Jump to address NNN.
        jump();
		break;

	case 0x2000://0x2NNN Call subroutine at NNN.
		stack[stack_pointer] = program_counter; //Store program counter on the stack
		//prevent stack overflow
		if(stack_pointer < 16)
		{
			program_counter = opcode & 0x0FFF;//Same as above opcode
			stack_pointer++;
		}
		else
			std::cout << "Stack error";
		break;

	case 0x3000://0x3XNN Skips the next instruction if VX equals NN.
		//X opcode  & 0x0F00, remove last byte >> 8
		//NN opcode & 0x00FF
		if(registers[opcode & 0x0F00 >>8] == opcode & 0x00FF)
			increment_pc();
		break;

	case 0x4000://0x4XNN Skips the next instruction if VX doesn't equal NN.
		if(registers[opcode & 0x0F00 >>8] != opcode & 0x00FF)
			increment_pc();
		break;

	case 0x5000://0x5XY0 Skips the next instruction if VX equals VY.
		//X opcode & 0x0F00, remove last byte >> 8
		//Y opcode & 0x00F0, remove last 4 bits >> 4
		if (registers[opcode & 0x0F00 >> 8] == registers[opcode & 0x00F0 >> 4])
			increment_pc();
		break;

	case 0x6000://0x6XNN Sets VX to NN.
		//X  opcode & 0x0F00 >> 8	NN opcode & 0x00FF
		registers[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
		break;

	case 0x7000://0x7XNN Adds NN to VX.
		registers[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
		break;

	case 0x8000://0x8
		//9 opcodes here determined by last nibble
		switch(opcode & 0x000F)
		{
		case 0x0000://0x8XY0 Sets VX to the value of VY.
			reg_to_reg();
			break;

		case 0x0001://0x8XY1 Sets VX to VX or VY.
            reg_to_reg_or();
			break;
		case 0x0002://0x8XY2 Sets VX to VX and VY.
            reg_to_reg_and();
			break;
		case 0x0003://0x8XY3 Sets VX to VX xor VY.
            reg_to_reg_xor();
            break;
        case 0x0004://0x8XY4 Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
            reg_to_reg_add();
            break;
        case 0x0005://0x8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            reg_to_reg_sub();
            break;
        case 0x0006://0x8XY6 Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
            reg_shift_rt();
            break;
        case 0x0007://0x8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            reg_to_reg_sub_inv();
            break;
        case 0x000E://0x8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
            reg_shift_lt();
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
			if(registers[opcode & 0x0F00 >> 8] != registers[opcode & 0x00F0 >> 4])
				increment_pc();
			break;
		default:
			opcodeError();
			break;
		}
		break;

	case 0xA000://0xANNN Sets I to the address NNN.
		index_register = opcode & 0x0FFF;
		break;

	case 0xB000://0xBNNN Jumps to the address NNN plus V0.
		program_counter = (opcode & 0x0FFF) + registers[0];
		break;

	case 0xC000://0xCXNN Sets VX to the result of a bitwise and operation on a random number and NN.
		registers[opcode & 0x0F00 >> 8] = (opcode & 0x00FF) & (rand()%0xFF);
		break;
	
	//Understand wtf is happening here
	case 0xD000://0xDXYN Sprites stored in memory at location in index register (I), 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it toggles the screen pixels). Sprites are drawn starting at position VX, VY. N is the number of 8bit rows that need to be drawn. If N is greater than 1, second line continues at position VX, VY+1, and so on.
	{
		unsigned short x = registers[opcode & 0x0F00 >> 8];
		unsigned short y = registers[opcode & 0x00F0 >> 4];
		unsigned short draw_height = opcode & 0x000F;
		unsigned short draw_pixel;
		
		registers[0xF] = 0;
		for(int y_line = 0; y_line < draw_height; y_line++)
		{
			draw_pixel = memory[index_register + y_line];
			for(int x_line = 0; x_line < 8; x_line++)
			{
				if((draw_pixel & (0x80 >> x_line)) != 0)
				{
					if(graphics[(x + x_line + ((y + y_line)*32))] == 1)
					{
						registers[0xF] = 1;
					}
					graphics[x + x_line + ((y + y_line)*32)] ^= 1;
				}
			}
		}
		
		draw_flag = true;
		break;
	}
	case 0xE000://0xE
		switch(opcode & 0x00FF)
		{
			//Im guessing V[x] > 0 pressed, V[x] = 0 not pressed
		case 0x009E://0xEX9E Skips the next instruction if the key stored in VX is pressed.
			if(registers[opcode & 0x0F00 >> 8] > 0)
				increment_pc();
			break;

		case 0x00A1://0xEXA1 Skips the next instruction if the key stored in VX isn't pressed.
			if(registers[opcode & 0x0F00 >> 8] == 0)
				increment_pc();
			break;
		default:
			opcodeError();
			break;
		}
		break;

	case 0xF000://0xF
		switch(opcode & 0x00FF)
		{
		case 0x0007://0xFX07 Sets VX to the value of the delay timer.
			registers[(opcode & 0x0F00) >> 8] = delay_timer;
			break;

		case 0x000A://0xFX0A A key press is awaited, and then stored in VX.
		{
			bool key_pressed = false;
			
			for(int input_index; input_index < 16; input_index++)
			{
				if(input[input_index] != 0)
				{
					registers[(opcode & 0x0F00) >> 8] = input_index;
					key_pressed = true;
				}
			}
			//I think this is right?... not sure
			if(!key_pressed)
				program_counter -= 2;
			break;
		}

		case 0x0015://0xFX15 Sets the delay timer to VX.
			delay_timer = registers[(opcode & 0x0F00) >> 8];
			break;

		case 0x0018://0xFX18 Sets the sound timer to VX.
			sound_timer = registers[(opcode & 0x0F00) >> 8];
			break;

		case 0x001E://0xFX1E Adds VX to I.
			index_register += registers[(opcode & 0x0F00) >> 8];
			if (index_register + registers[(opcode & 0x0F00) >> 8] > 0xFFF) //VF =1 with overflow, 0 o.w.
				registers[0xF]=1;
			else
				registers[0xF]=0;
			break;
		
		case 0x0029://0xFX29 Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
			index_register += registers[(opcode & 0x0F00) >> 8]*0x5;
			break;

		case 0x0033://0xFX33 Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
			//stole this too
			memory[index_register]   = registers[(opcode & 0x0F00)>>8]/100;
			memory[index_register+1] = (registers[(opcode & 0x0F00)>>8]/ 10) %10;
			memory[index_register+2] = (registers[(opcode & 0x0F00)>>8]%100) %10;	
			break;

		case 0x0055://0xFX55 Stores V0 to VX in memory starting at address I.
			for(int register_counter = 0; register_counter < (opcode & 0x0F00 >> 8); register_counter++)
				memory[index_register + register_counter] = registers[register_counter];
			//I = I + X + 1.
			index_register += ((opcode & 0x0F00) >> 8) + 1;
			break;

		case 0x0065://0xFX65 Fills V0 to VX with values from memory starting at address I.
			for(int register_counter = 0; register_counter < (opcode & 0x0F00 >> 8); register_counter++)
				registers[register_counter] = memory[index_register];
			//I = I + X + 1.
			index_register += ((opcode & 0x0F00) >> 8) + 1;
			break;
		default:
			opcodeError();
			break;
		}
		break;


	default:
		opcodeError();
		break;
	}



	//lets return -1 or something for invalid opcodes and halt exectuion
	return 0;
}

void Chip8Emulator::debugGraphics()
{
    std::cout << "GRAPHICS" << std::endl;
	for (int y = 0; y < 64; y++)
	{
		for(int x = 0; x < 32; x++)
		{
			if(graphics[(y*32)+x] == 0)
				std::cout << "X";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}
