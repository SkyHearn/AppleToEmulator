#include "6502-internal.h"

void _proceed(CPU* cpu) 
{
    cpu->ab = cpu->PC;
    cpu->PC++;
    cpu->tick_cb(cpu, _calculate_pin_status(*cpu));
}

void _set_byte_zp(CPU* cpu, uint8_t value) 
{
    cpu->ab = cpu->db;
    cpu->rw = 0;
    cpu->db = value;
    cpu->tick_cb(cpu, _calculate_pin_status(*cpu));
    cpu->rw = 1;
}

void _get_byte(CPU* cpu, uint16_t address) 
{
    cpu->ab = address;
    cpu->tick_cb(cpu, _calculate_pin_status(*cpu));
}

PINS _calculate_pin_status(CPU cpu) 
{
    PINS pin_status;

    pin_status.ab = cpu.ab;
    pin_status.db = cpu.db;
    pin_status.rw = cpu.rw;

    return pin_status;
}

void _set_zn_registers(CPU* cpu, uint8_t value) 
{
    cpu->Z = value == 0 ? 1 : 0;
    cpu->N = (1 & value) == 1 ? 1 : 0;
}