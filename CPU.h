#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>

#define NUM_REGISTERS 16
#define STACK_SIZE 16
#define NUM_KEYS 16
#define GFX_WIDTH 64
#define GFX_HEIGHT 32
#define MEM_SIZE 4096

using namespace std;

class CPU{
private:
	unsigned short opcode;		//Opcodes are 2 bytes long, shorts are also 2 bytes 
	
	unsigned char V[NUM_REGISTERS];		//Registers in Chip-8 are V0-VF, so V[x] is register
	unsigned short I;			//Index register
	unsigned short pc;			//Program Counter
	

	/*	Memory Map:
			0x000-0x1FF - Chip 8 interpreter (Contains font set in emu)
			0x050-0x0A0 - Used for built in 4x5 pixel font set (0-F)
			0x200-0xFFF - Program ROM and work RAM
	*/
	unsigned char delay_timer;	//Count at 60Hz
	unsigned char sound_timer;	//when 0 the system's buzzer goes off

	unsigned short stack[STACK_SIZE];	//16 level stack, used to store memory location before jump
	unsigned short sp;			//Stack pointer, points to memory location before jump
	unsigned char key[NUM_KEYS];		//store current state of key on keypad
	
	void invalidOpcode();
	void checkTimers();
	void fetchOpcode();
	void decodeAndExecute();
	
public:
	unsigned char gfx[GFX_WIDTH * GFX_HEIGHT]; //Draws a 64x32 screen. 2048 pixels. (0 for off, 1 for on)
	CPU();
	bool drawFlag;
	unsigned char memory[MEM_SIZE];	//4K of memory, each char == 1 byte
	SDL_Event event; 

	void initialize();
	void emulateCycle();
	void setKeys();

	void dumpRegisters();
	

	void opcode_0NNN();
	void opcode_00E0();
	void opcode_00EE();
	void opcode_1NNN();
	void opcode_2NNN();
	void opcode_3XNN();
	void opcode_4XNN();
	void opcode_5XY0();
	void opcode_6XNN();
	void opcode_7XNN();
	void opcode_8XY0();
	void opcode_8XY1();
	void opcode_8XY2();
	void opcode_8XY3();
	void opcode_8XY4();
	void opcode_8XY5();
	void opcode_8XY6();
	void opcode_8XY7();
	void opcode_8XYE();
	void opcode_9XY0();
	void opcode_ANNN();
	void opcode_BNNN();
	void opcode_CXNN();
	void opcode_DXYN();
	void opcode_EX9E();
	void opcode_EXA1();
	void opcode_FX07();
	void opcode_FX0A();
	void opcode_FX15();
	void opcode_FX18();
	void opcode_FX1E();
	void opcode_FX29();
	void opcode_FX33();
	void opcode_FX55();
	void opcode_FX65();
	

};