#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

struct CPU 
{
	Word PC;        // program counter
	Byte SP = 0;    // stack pointer. add 0x0100 to it to get the stack location.

	Byte A = 0, X = 0, Y = 0; // A, X, and Y registers.

	Byte C : 1;  // Carry flag.
	Byte Z : 1;  // Zero flag.
	Byte I : 1;  // Interrupt disable.
	Byte D : 1;  // Decimal mode.
	Byte B : 1;  // Break Command
	Byte O : 1;  // Overflow Flag
	Byte N : 1;  // Negitive Flag.

	Byte mem[4000] = {0};
};

// initlize the CPU
void initCPU(CPU* cpu)
{
	cpu->C = 0;
	cpu->Z = 0;
	cpu->I = 0;
	cpu->D = 0;
	cpu->B = 0;
	cpu->O = 0;
	cpu->N = 0;

	cpu->mem[0x200] = 0xa9;
	cpu->mem[0x201] = 0xff;

	cpu->PC = 0x200; // TEMPORARY THIS IS NOT HOW THE APPLE II WORKS
}

void execute(CPU* cpu, unsigned int cycles) 
{
	while (cycles != 0) 
	{
		switch (cpu->mem[cpu->PC])
		{

		// LDA immidiate addressing mode
		case 0xa9:
		{
			cpu->A = cpu->mem[cpu->PC + 1];
			Byte A_cpy = cpu->A; // copy A so that we can do bitwise on it.

			if (cpu->A == 0) cpu->Z = 1; // set the zero flag
			if ((0b00000001 & A_cpy) == 0b00000001) cpu->N = 1; // set the negitive flag if b7 is 1

			--cycles;
			--cycles; // decriment the correct number of cycles

			cpu->PC = cpu->PC += 2;  // move the program counter

			break;
		}

		default:
			std::cout << "Instruction not handled \n";
		}
	}
}

int main()
{
	CPU cpu;
	initCPU(&cpu);

	execute(&cpu, 2);

	return 0;
}