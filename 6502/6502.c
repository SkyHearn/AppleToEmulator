#include "6502-internal.h"
#include <stdio.h>

void (*tick_cb) (CPU* cpu, PINS);

void set_tick_cb(void (*cb) (CPU* cpu, PINS))
{
    tick_cb = cb;
}

void _tick_system(CPU* cpu, PINS ps)
{
    tick_cb(cpu, ps);
}

void _proceed(CPU* cpu) 
{
    cpu->ab = cpu->PC;
    cpu->PC++;
    _tick_system(cpu, _calculate_pin_status(*cpu));
}

void _set_byte(CPU* cpu, uint8_t value) 
{
    cpu->ab = cpu->db;
    cpu->rw = 0;
    cpu->db = value;
    _tick_system(cpu, _calculate_pin_status(*cpu));
    cpu->rw = 1;
}

void set_db(CPU* cpu, uint8_t value) { cpu->db = value; }

PINS _calculate_pin_status(CPU cpu) 
{
    PINS pin_status;

    pin_status.ab = cpu.ab;
    pin_status.db = cpu.db;
    pin_status.rw = cpu.rw;

    return pin_status;
}

CPU initialize_cpu(uint8_t mem[]) 
{
    CPU cpu;
    
    // Set the program counter to the address pointed to in the reset vector
    cpu.PC = (mem[0xFFFD] * 256) + mem[0xFFFC];
    
    cpu.SP = 0;
    cpu.A = cpu.X = cpu.Y = cpu.C = cpu.Z = cpu.D = cpu.B = cpu.O = cpu.N = 0;
    cpu.I = 1;
    
    cpu.ab = 0;
    cpu.db = 0;
    
    cpu.rw = 1;

    return cpu;
}

void execute(CPU* cpu, unsigned int cycles)
{
    while (cycles != 0)
    {
        _proceed(cpu);
        --cycles;

        switch (cpu->db)
        {

            // LDA immidiate addressing mode
            case 0xa9:
            {
                _proceed(cpu); 
                --cycles;

                cpu->A = cpu->db;
            
                // set the zero and negitive flags to their proper values
                cpu->Z = cpu->A == 0 ? 1 : 0;
                cpu->N = (0b00000001 & cpu->A) == 0b00000001 ? 1 : 0;
            
                break;
            }
        
            case 0x85:
            {
                _proceed(cpu);
                --cycles;
            
                _set_byte(cpu, cpu->A);
                --cycles;

                break;
            }

            default:
            {
                printf("Unknown Instruction\n");
            }
        }
    }
}
