#include <iostream>
#include "6502.h"

int main()
{
	processor::CPU cpu;

	cpu.PC = 0x200;
	cpu.A = 0;
	cpu.Z = 0;
	cpu.I = 0;
	cpu.D = 0;
	cpu.B = 0;
	cpu.O = 0;
	cpu.N = 0;

	cpu.mem[0x200] = 0xa9;
	cpu.mem[0x201] = 0xff;

	processor::execute(&cpu, 2);

	return 0;
}