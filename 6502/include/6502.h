#ifndef _6502_H
#define _6502_H

#include <stdint.h>

struct CPU
{
    uint16_t PC;     // program counter
    uint8_t SP;     // stack pointer. add 0x0100 to it to get the stack location.

    uint8_t A, X, Y; // A, X, and Y registers.

    uint8_t C : 1;  // Carry flag.
    uint8_t Z : 1;  // Zero flag.
    uint8_t I : 1;  // Interrupt disable.
    uint8_t D : 1;  // Decimal mode.
    uint8_t B : 1;  // Break Command
    uint8_t O : 1;  // Overflow Flag
    uint8_t N : 1;  // Negitive Flag.
    
    uint16_t ab; // Address bus.
    uint8_t db; // Data Bus.
    uint8_t rw; // read write pin.
};

typedef struct CPU CPU;

struct PINS
{
    uint16_t ab;
    uint8_t db;
    uint8_t rw;
};

typedef struct PINS PINS;

// set the databus.
void set_db(CPU* cpu, uint8_t value);

// execute a number of cycles on the cpu.
void execute(CPU* cpu, unsigned int cycles);

// set the function that is called back for each cycle of the CPU.
void set_tick_cb(void (*cb) (CPU *cpu, PINS));

// initialize the cpu to it's proper starting values, 
// including setting the program counter to the address
// pointed to at the reset vector.
CPU initialize_cpu(uint8_t mem[]);

#endif
