#include <iostream>

struct CPU 
{
	using Byte = unsigned char;
	using Word = unsigned short;

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

	Byte mem[64000] = {0};
};

// initlize the CPU
void initCPU(CPU cpu)
{
	cpu.C = 0;
	cpu.Z = 0;
	cpu.I = 0;
	cpu.D = 0;
	cpu.B = 0;
	cpu.O = 0;
	cpu.N = 0;

	cpu.PC = 0x200; // TEMPORARY THIS IS NOT HOW THE APPLE II WORKS
}

void execute(CPU cpu, unsigned int cycles) 
{
	while (cycles != 0) 
	{
		switch (cpu.mem[cpu.PC])
		{
		case 0xa9:
			cpu.A = cpu.mem[cpu.PC + 1];
			if (cpu.A = 0) cpu.Z = 1;
			if (0b00000001 & cpu.A) cpu.Z = 1;
			--cycles;
			--cycles;
			break;
		default:
			std::cout << "Instruction not handled \n";
		}
	}
}

int main()
{
	CPU cpu;
	initCPU(cpu);

	execute(cpu, 2);

	return 0;
}