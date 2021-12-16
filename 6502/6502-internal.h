#ifndef _6502_INTERNAL_H
#define _6502_INTERNAL_H

#include "include/6502.h"

// calls the function pointed to at *tick_cb
void _tick_system(CPU*, PINS);

// returns the status of pins on the cpu
PINS _calculate_pin_status(CPU);

// retreive what is stored in memory on the next address
// advance the program counter
void _proceed(CPU* cpu);

// set a byte in the zero page, calling the system as needed.
void _set_byte_zp(CPU* cpu, uint8_t value);

// sets the data bus to the value of a byte in the zero page.
void _get_byte_zp(CPU* cpu, uint8_t address);

#endif
