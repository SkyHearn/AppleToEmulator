#pragma once

namespace processor 
{
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

		Byte mem[4000] = { 0 };
	};

	// execute a given number of cycles on the cpu.
	void execute(CPU* cpu, unsigned int cycles);
}