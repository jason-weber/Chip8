#include "CPU.h"

CPU::CPU(){
	pc = 0x200; //In Chip-8, program counter starts at address 0x200
	opcode = 0;
	I = 0;
	sp = 0;
	drawFlag = false;

	//Clear stack and registers V[0] - V[F]
	for(int i = 0; i < 16; i++){
		stack[i] = 0;
		V[i] = 0;
	}

	//Clear gfx
	for(int i = 0; i < (GFX_WIDTH * GFX_HEIGHT); i++){
		gfx[i] = 0;
	}
	//Clear memory
	for(int i = 0; i < MEM_SIZE; i++){
		memory[i] = 0;
	}

	//Reset keys
	for(int i = 0; i < NUM_KEYS; i++){
		key[i] = 0;
	}

	//Reset timers
	delay_timer = 0;
	sound_timer = 0;

	//Chip 8 internal fontset
	unsigned char fontset[80] ={ 
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	//Copy font set into memory
	for(int i = 0; i < 80; i++){
		memory[i] = fontset[i];
	}
}

/*
* Checks for a key press 0-f
* Also checks for SDL_QUIT
**/
void CPU::setKeys(){
	while(SDL_PollEvent(&event)){
		switch(event.type){
		case SDL_QUIT:
			exit(0);
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
			case SDLK_0:
				key[0] = 1;
				break;
			case SDLK_1:
				key[1] = 1;
				break;
			case SDLK_2:
				key[2] = 1;
				break;
			case SDLK_3:
				key[3] = 1;
				break;
			case SDLK_4:
				key[4] = 1;
				break;
			case SDLK_5:
				key[5] = 1;
				break;
			case SDLK_6:
				key[6] = 1;
				break;
			case SDLK_7:
				key[7] = 1;
				break;
			case SDLK_8:
				key[8] = 1;
				break;
			case SDLK_9:
				key[9] = 1;
				break;
			case SDLK_a:
				key[10] = 1;
				break;
			case SDLK_b:
				key[11] = 1;
				break;
			case SDLK_c:
				key[12] = 1;
				break;
			case SDLK_d:
				key[13] = 1;
				break;
			case SDLK_e:
				key[14] = 1;
				break;
			case SDLK_f:
				key[15] = 1;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_0:
				key[0] = 0;
				break;
			case SDLK_1:
				key[1] = 0;
				break;
			case SDLK_2:
				key[2] = 0;
				break;
			case SDLK_3:
				key[3] = 0;
				break;
			case SDLK_4:
				key[4] = 0;
				break;
			case SDLK_5:
				key[5] = 0;
				break;
			case SDLK_6:
				key[6] = 0;
				break;
			case SDLK_7:
				key[7] = 0;
				break;
			case SDLK_8:
				key[8] = 0;
				break;
			case SDLK_9:
				key[9] = 0;
				break;
			case SDLK_a:
				key[10] = 0;
				break;
			case SDLK_b:
				key[11] = 0;
				break;
			case SDLK_c:
				key[12] = 0;
				break;
			case SDLK_d:
				key[13] = 0;
				break;
			case SDLK_e:
				key[14] = 0;
				break;
			case SDLK_f:
				key[15] = 0;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void CPU::dumpRegisters(){
	cout << "Registers: " << endl;
	for(int i = 0; i < sizeof(V); i++){
		cout << "V[" << i << "]: " << int(memory[i]) << endl;
	}
}

/*
* Switch statement with 35 cases.
* One for each opcode.
* Calls method corresponding to opcode.
**/
void CPU::decodeAndExecute(){
	switch(opcode & 0xF000){
	case 0x0000:
		switch(opcode & 0x00FF){
		case 0xEE:
			opcode_00EE();
			break;
		case 0xE0:
			opcode_00E0();
			break;
		default:
			invalidOpcode();
			break;
		}
		break;
	case 0x1000:
		opcode_1NNN();
		break;
	case 0x2000:
		opcode_2NNN();
		break;
	case 0x3000:
		opcode_3XNN();
		break;
	case 0x4000:
		opcode_4XNN();
		break;
	case 0x5000:
		opcode_5XY0();
		break;
	case 0x6000:
		opcode_6XNN();
		break;
	case 0x7000:
		opcode_7XNN();
		break;
	case 0x8000:
		switch(opcode & 0x000F){
		case 0x0:
			opcode_8XY0();
			break;
		case 0x1:
			opcode_8XY1();
			break;
		case 0x2:
			opcode_8XY2();
			break;
		case 0x3:
			opcode_8XY3();
			break;
		case 0x4:
			opcode_8XY4();
			break;
		case 0x5:
			opcode_8XY5();
			break;
		case 0x6:
			opcode_8XY6();
			break;
		case 0x7:
			opcode_8XY7();
			break;
		case 0xE:
			opcode_8XYE();
			break;
		default:
			invalidOpcode();
			break;
		}
		break;
	case 0x9000:
		opcode_9XY0();
		break;
	case 0xA000:
		opcode_ANNN();
		break;
	case 0xB000:
		opcode_BNNN();
		break;
	case 0xC000:
		opcode_CXNN();
		break;
	case 0xD000:
		opcode_DXYN();
		break;
	case 0xE000:
		switch(opcode & 0x00FF){
		case 0x009E:
			opcode_EX9E();
			break;
		case 0x00A1:
			opcode_EXA1();
			break;
		default:
			invalidOpcode();
			break;
		}
		break;
	case 0xF000:
		switch(opcode & 0x00FF){
		case 0x07:
			opcode_FX07();
			break;
		case 0x0A:
			opcode_FX0A();
			break;
		case 0x15:
			opcode_FX15();
			break;
		case 0x18:
			opcode_FX18();
			break;
		case 0x1E:
			opcode_FX1E();
			break;
		case 0x29:
			opcode_FX29();
			break;
		case 0x33:
			opcode_FX33();
			break;
		case 0x55:
			opcode_FX55();
			break;
		case 0x65:
			opcode_FX65();
			break;
		default:
			invalidOpcode();
			break;
		}
		break;
	default:
		invalidOpcode();
		break;
	}
	cout << "Executed opcode: " << hex << opcode << endl;
	/*dumpMemory();
	cin.get();*/
}

/*
* Reads the next opcode from memory[pc] and memory[pc + 1]
**/
void CPU::fetchOpcode(){
	//Gets first half of opcode from memory at pc
	opcode = memory[pc];

	//Shifts that left to fit second half of opcode
	opcode <<= 8;

	//Or together the first and second half of opcode
	opcode |= memory[pc + 1];
}

/*
* Gathers next opcode, executes it, and updates timers
**/
void CPU::emulateCycle(){
	//Gets opcode from memory at pc
	fetchOpcode();
	
	//Decode opcode
	//Giant switch statement for each opcode
	decodeAndExecute();

	//Update timers
	checkTimers();
}

/*
* Checks delay_timer and sound_timer and decrements them
* if they are greater than 0. sound_timer prints "beep\n" when 1
**/
void CPU::checkTimers(){
	if(delay_timer > 0){
		--delay_timer;
	}
	if(sound_timer > 0){
		if(sound_timer == 1){
			cout << "Beep\n";
		}
		--sound_timer;
	}
}

/*
* Called when no opcode case matches given opcode
**/
void CPU::invalidOpcode(){
	cout << "Invalid opcode: 0x" << hex << opcode << "\n";
	cout << "Press enter to continue.";
	cin.get();
}

void CPU::opcode_0NNN(){
	
}

/*
* Clears the screen. 
**/
void CPU::opcode_00E0(){
	for(int i = 0; i < 2048; i++){
		gfx[i] = 0;
	}
	pc += 2;
	drawFlag = true;
}

/*
* Returns from a subroutine
**/
void CPU::opcode_00EE(){
	pc = stack[sp];
	--sp;
	pc += 2;
}

/*
*	Jumps to address NNN
**/
void CPU::opcode_1NNN(){
	pc = opcode & 0x0FFF;
}

/*
* Calls a subroutine at address NNN
**/
void CPU::opcode_2NNN(){
	++sp;
	stack[sp] = pc;
	pc = opcode & 0x0FFF;
}

/*
* Skip the next instruction if V[X] == NN
**/
void CPU::opcode_3XNN(){
	if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Skip the next instruction if V[X] != NN
**/
void CPU::opcode_4XNN(){
	if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Skip the next instruction if V[X] == V[Y]
**/
void CPU::opcode_5XY0(){
	if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Set V[X] to NN
**/
void CPU::opcode_6XNN(){
	V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
	pc += 2;
}

/*
* Add NN to V[X]
*/
void CPU::opcode_7XNN(){
	V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
	pc += 2;
}

/*
* Set V[X] to V[Y]
*/
void CPU::opcode_8XY0(){
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

/*
* Set V[X] to (V[X] OR V[Y])
**/
void CPU::opcode_8XY1(){
	V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) > 4]);
	pc += 2;
}
/*
* Set V[X] to (V[X] AND V[Y])
**/
void CPU::opcode_8XY2(){
	V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) > 4]);
	pc += 2;
}

/*
* Set V[X] to (V[X] XOR V[Y])
**/
void CPU::opcode_8XY3(){
	V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) > 4]);
	pc += 2;
}

/*
* Add V[Y] to V[X] and sets Carry flag (V[0xF])
**/
void CPU::opcode_8XY4(){
	if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])){
		V[0xF] = 1;
	}
	else{
		V[0xF] = 0;
	}
	V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

/*
* Subtract V[Y] from V[X]. Set V[F] to 0 if borrow, 1 if not
**/
void CPU::opcode_8XY5(){
	if(V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]){
		V[0xF] = 0;
	}
	else{
		V[0xF] = 1;
	}
	V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
	pc += 2;
}

/*
* Shifts V[X] right 1 bit. Stores least significant bit in V[F]
**/
void CPU::opcode_8XY6(){
	V[0xF] = (V[(opcode & 0x0F00) >> 8] & 1);
	V[(opcode & 0x0F00) >> 8] >>= 1;
	pc += 2;
}

/*
* Subtracts V[X] from V[Y] and stores in V[X]
**/
void CPU::opcode_8XY7(){
	if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
		V[0xF] = 0;
	}
	else{
		V[0xF] = 1;
	}
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

/*
* Shift V[X] left by one. V[F] set to value of most significant bit
**/
void CPU::opcode_8XYE(){
	//Gets most significant bit. shifts it 7 places to the ones position
	V[0xF] = (V[(opcode & 0x0FF0) >> 8] & 0x80) >> 7; 
	V[(opcode & 0x0F00) >> 8] <<= 0x1; 
	pc += 2;
}

/*
* Skips the next instruction if V[X] != V[Y]
**/
void CPU::opcode_9XY0(){
	if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Sets I to address NNN
**/
void CPU::opcode_ANNN(){
	I = (opcode & 0x0FFF);
	pc += 2;
}

/*
* Jumps to address NNN + V[0]
**/
void CPU::opcode_BNNN(){
	pc = ((opcode & 0x0FFF) + V[0]);
}

/*
* Stores RANDOM byte AND NN into V[X]
**/
void CPU::opcode_CXNN(){
	V[(opcode & 0x0F00) >> 8] = ((unsigned char)rand()) & (opcode & 0x00FF);
	pc += 2;
}

/*
* Draws sprite at coordinate (V[X], V[Y])
* Each sprite has width 8px by height Npx
* Sprites start at memory[I]. I does NOT change after execution.
* V[F] set to 1 if any pixels flipped from set to unset, 0 if not
**/
void CPU::opcode_DXYN(){
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;
 
	V[0xF] = 0;
	for (int yline = 0; yline < height; yline++){
		pixel = memory[I + yline];
		for(int xline = 0; xline < 8; xline++){
			if((pixel & (0x80 >> xline)) != 0){
				if(gfx[(x + xline + ((y + yline) * 64))] == 1){
					V[0xF] = 1;
				}
				gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
		}
	}
	drawFlag = true;
	pc += 2;	
}

/*
* Skips next instruction if key in V[X] is pressed
**/
void CPU::opcode_EX9E(){
	if(key[V[(opcode & 0x0F00) >> 8]] != 0){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Skips next instruction if key in V[X] isn't pressed
**/
void CPU::opcode_EXA1(){
	if(key[V[(opcode & 0x0F00) >> 8]] == 0){
		pc += 4;
	}
	else{
		pc += 2;
	}
}

/*
* Sets V[X] to delay_timer
**/
void CPU::opcode_FX07(){
	V[(opcode & 0x0F00) >> 8] = delay_timer;
	pc += 2;
}

/*
* Awaits key press, then stores key in V[X]
**/
void CPU::opcode_FX0A(){
	char key;
	cin >> hex >> key;
	if(key >= 0x0 && key <= 0xF){
		V[opcode & 0x0F000 >> 8] = key;
	}
	else{
		cout << "Invalid Key entered" << endl;
	}
	pc += 2;
}

/*
* Sets delay timer to VX
**/
void CPU::opcode_FX15(){
	delay_timer = V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

/*
* Sets sound timer to V[X]
**/
void CPU::opcode_FX18(){
	sound_timer = V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

/*
* Adds V[X] to I
**/
void CPU::opcode_FX1E(){
	if(0xFFF - V[(opcode & 0x0F00) >> 8] < I){
		V[0xF] = 1;
	}
	else{
		V[0xF] = 0;
	}
	I += V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

/*
* Sets I to the location of the sprite for character in V[X]
**/
void CPU::opcode_FX29(){
	I = V[(opcode & 0x0F00) >> 8];
	pc += 2;
}

/*
* Store binary coded decimal representation of V[X]
* Hundreds place at memory[I]
* Tens place at memory[I + 1]
* Ones place at memory[I + 2]
**/
void CPU::opcode_FX33(){
	memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
	memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
	pc += 2;
}

/*
* Takes values V[0] through V[X] and stores them in succession in memory,
* starting at memory[I]
**/
void CPU::opcode_FX55(){
	for(int i = 0; i < ((opcode & 0x0F00) >> 8); i++){
		memory[I + i] = V[i];
	}
	pc += 2;
}

/*
* Fills values V[0] through V[X] with values from memory,
* starting at memory[I]
**/
void CPU::opcode_FX65(){
	for(int i = 0; i < ((opcode & 0x0F00) >> 8); i++){
		V[i] = memory[I + i];
	}
	pc += 2;
}

/*
* Allows pixel by pixel editing on SDL_SURFACE
**/
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

bool loadRom(CPU* chip8){
	string filename;
	cout << "Enter the name of the program to load: ";
	cin >> filename;
	cin.get();
	

	cout << "Loading %s...\n" << filename << endl;
	FILE *pFile;
	pFile = fopen(filename.c_str(), "rb");

	if(!pFile)
	{
		printf("Rom not found\n\n");
		return false;
	}

	// Check file size
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);

	printf("Filesize: %d\n", (int)lSize);
	// Allocate memory to contain the whole file
	char *buffer = (char*)malloc(sizeof(char) * lSize);

	if(!buffer)
	{
		printf("Error allocating rom data\n\n");
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, lSize, pFile);

	if(result != lSize)
	{
		printf("Error reading rom data\n\n");
		return false;
	}

	// Copy buffer to Chip8 memory
	if((4096 - 512) > lSize){
		for(int i = 0; i < lSize; ++i)
			chip8->memory[i + 512] = buffer[i];
	}
	else{
		printf("Error: ROM too large for memory");
	}

	// Close file and free buffer
	fclose(pFile);
	free(buffer);
}

void drawScreen(CPU* chip8, SDL_Surface* window){
	if(SDL_LockSurface(window) == 0){
				for(int x = 0; x < 64; x++){
					for(int y = 0; y < 32; y++){

						Uint8 gfx = chip8->gfx[x + y * 64];
						for(int winX = 0; winX < 10; winX++){
							for(int winY = 0; winY < 10; winY++){
								for(int pix = 7; pix >= 0; pix--){
									putpixel(window, x * 10 + winX, y * 10 + winY, (gfx >> pix)* 150);
								}
							}
						}
					}
				}
			}
			else{
				cout << "SURFACE COULD NOT BE LOCKED" << endl;
				cin.get();
			}
			SDL_UnlockSurface(window);
			SDL_UpdateRect(window, 0, 0, 0, 0);
			chip8->drawFlag = false;
}

int main(int argc, char **argv){
	CPU* chip8 = new CPU(); // Our Chip-8 CPU
	
	loadRom(chip8);

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* window = SDL_SetVideoMode(640, 320, 32, SDL_SWSURFACE);
	

	for(;;){
		chip8->emulateCycle();	//Grabs opcode, decodes it, and executes it

		//If draw flag is set, update screen
		if(chip8->drawFlag){
			drawScreen(chip8, window);
		}
		
		//Store key press state (press and release)
		chip8->setKeys();
	}

	SDL_Quit();
}



