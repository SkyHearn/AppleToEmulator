#ifndef _6502_INTERNAL_H
#define _6502_INTERNAL_H

#include "include/6502.h"

// returns the status of pins on the cpu
PINS _calculate_pin_status(CPU);

// retreive what is stored in memory on the next address
// advance the program counter
void _proceed(CPU* cpu);

// set a byte in the zero page, calling the system as needed.
void _set_byte_zp(CPU* cpu, uint8_t value);

// sets the data bus to the value of a byte stored in memory.
void _get_byte(CPU* cpu, uint16_t address);

// set the values of the negative and zero registers.
void _set_zn_registers(CPU* cpu, uint8_t value);
#endif
