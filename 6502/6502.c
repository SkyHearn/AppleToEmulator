#include "6502-internal.h"
#include <stdlib.h>
#include <stdio.h>

void set_db(CPU* cpu, uint8_t value) { cpu->db = value; }

CPU initialize_cpu(uint8_t mem[], void (*cb) (CPU* cpu, PINS)) 
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

    cpu.tick_cb = cb;

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
                cpu->tick_cb(cpu, _calculate_pin_status(*cpu));
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

                _proceed(cpu);
                cycles--;
                addr += (((uint16_t)cpu->db) << 8);                
                
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
                cpu->tick_cb(cpu, _calculate_pin_status(*cpu));
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
