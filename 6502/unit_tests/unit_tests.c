#include "unit_tests.h"
#include <stdlib.h>

uint8_t mem[0xFFFF];

uint8_t get_byte(uint16_t address) 
{
    return mem[address];
}

int assert_failed(int line, char *file) 
{
    printf("\x1b[31m FAILED LINE:%i FILE:%s \x1b[0m\n", line, file);
    return 1;
}

int assert_passed(int line, char *file)
{
    printf("\x1b[32m PASSED LINE:%i FILE:%s \x1b[0m\n", line, file);
    return 0;
}

int main(int argc, char** argv) 
{
    if (argc > 2) 
    {
        printf("Too many arguments. \n");
        return 2;
    }
    
    if (argc == 1) 
    {
        printf("Must specify a test to run \n");
        return 2;
    }
    
    char* str;
    long int test_num = strtol(argv[1], &str , 10);

    switch (test_num) 
    {
        case 0:
            return LDA_immediate_test();
        case 1:
            return LDA_neg_flag_test();
        case 2:
            return LDA_z_flag_test();
        case 3:
            return STA_test();
        default:
            printf("No test with number: %ld \n", test_num);
            return 2;
    }
}

void on_system_tick(CPU *cpu, PINS ps) 
{   
    if (ps.rw == 1) 
    {
        set_db(cpu, get_byte(ps.ab));
    } else 
    {
        mem[ps.ab] = ps.db;
    }
}

int LDA_immediate_test() 
{
    mem[0] = 0xa9;
    mem[1] = 0xFF;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem);
    set_tick_cb(&on_system_tick);

    execute(&cpu, 2);

    return assert(cpu.A == 0xFF);
}

int LDA_neg_flag_test()
{
    mem[0] = 0xa9;
    mem[1] = 0xFF;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem);

    set_tick_cb(&on_system_tick);
    execute(&cpu, 2);

    return assert(cpu.N == 1);
}

// zero flag test
int LDA_z_flag_test()
{
    mem[0] = 0xa9;
    mem[1] = 0x00;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem);

    set_tick_cb(&on_system_tick);
    execute(&cpu, 2);

    return assert(cpu.Z == 1);
}

int STA_test()
{
    mem[0] = 0xa9;
    mem[1] = 0x69; // nice
    mem[2] = 0x85;
    mem[3] = 0x00;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem);

    set_tick_cb(&on_system_tick);
    execute(&cpu, 5);
    
    return assert(mem[0] == 0x69);
}
