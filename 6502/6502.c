#include "6502-internal.h"
#include <stdlib.h>
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

void _set_byte_zp(CPU* cpu, uint8_t value) 
{
    cpu->ab = cpu->db;
    cpu->rw = 0;
    cpu->db = value;
    _tick_system(cpu, _calculate_pin_status(*cpu));
    cpu->rw = 1;
}

void _get_byte(CPU* cpu, uint16_t address) 
{
    cpu->ab = address;
    _tick_system(cpu, _calculate_pin_status(*cpu));
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
                cpu->N = (1 & cpu->A) == 1 ? 1 : 0;
            
                break;
            }
            
            // LDA ZP
            case 0xa5:
            {
               _proceed(cpu);
               --cycles;
               
               _get_byte(cpu, cpu->db);
               --cycles;
               
               cpu->A = cpu->db;

               cpu->Z = cpu->A == 0 ? 1 : 0;
               cpu->N = (1 & cpu->A) == 1 ? 1 : 0;

               break;
            }
            
            // LDA ZP, X
            case 0xb5:
            {
                _proceed(cpu);
                --cycles;
                
                cpu->ab = cpu->db;
                cpu->PC++;
                _tick_system(cpu, _calculate_pin_status(*cpu));
                --cycles;
                
                _get_byte(cpu, cpu->X + cpu->db);
                --cycles;
                
                cpu->A = cpu->db;

                cpu->Z = cpu->A == 0 ? 1 : 0;
                cpu->N = (1 & cpu->A) == 1 ? 1 : 0;

                break;
            }

            // LDA absolute
            case 0xAD:
            {
                uint16_t addr = 0;
                
                _proceed(cpu);
                cycles--; 

                addr += cpu->db;
                addr <<= 8;

                _proceed(cpu);
                cycles--;
                
                _get_byte(cpu, addr);
                cycles--;

                cpu->A = cpu->db;

                cpu->Z = cpu->A == 0 ? 1 : 0;
                cpu->N = (1 & cpu->A) == 1 ? 1 : 0;

                break;
            }

            // LDX #
            case 0xa2:
            {
                _proceed(cpu);
                --cycles;
                
                cpu->X = cpu->db;

                cpu->Z = cpu->X == 0 ? 1 : 0;
                cpu->N = (1 & cpu->X) == 1 ? 1 : 0;

                break;
            }

            // LDX ZP
            case 0xa6:
            {
                _proceed(cpu);
                --cycles;
               
                _get_byte(cpu, cpu->db);
                --cycles;
                
                cpu->X = cpu->db;

                cpu->Z = cpu->X == 0 ? 1 : 0;
                cpu->N = (1 & cpu->X) == 1 ? 1 : 0;

                break;           
            }

            // LDX ZP, Y
            case 0xb6:
            {
                _proceed(cpu);
                --cycles;
                
                cpu->ab = cpu->db;
                cpu->PC++;
                _tick_system(cpu, _calculate_pin_status(*cpu));
                --cycles;
                
                _get_byte(cpu, (uint8_t) cpu->Y + cpu->db);
                --cycles;

                cpu->X = cpu->db;

                cpu->Z = cpu->X == 0 ? 1 : 0;
                cpu->N = (1 & cpu->X) == 1 ? 1 : 0;

                break;
            }

            case 0x85:
            {
                _proceed(cpu);
                --cycles;
            
                _set_byte_zp(cpu, cpu->A);
                --cycles;

                break;
            }

            default:
            {
                //TODO: Replace with an "Unknown Instruction Handler calllback"
                printf("Unknown Instruction: %i\n", cpu->db);
                exit(1);
            }
        }
    }
}
