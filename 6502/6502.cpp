#include "6502.h"
#include <iostream>

namespace processor 
{

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
				std::cout << "Unknown Instruction\n";
			}
		}
	}
}