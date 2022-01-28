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
     
    printf("running test_num: %li\n", test_num);
    switch (test_num) 
    {
        case 0:
            LDA_immediate_test();
            break;
        case 1:
            LDA_neg_flag_test();
            break;
        case 2:
            LDA_z_flag_test();
            break;
        case 3:
            STA_test();
            break;
        case 4:
            LDA_zp_test();
            break;
        case 5:
            LDA_zp_X_test();
            break;
        case 6:
            LDX_immediate_test();
            break;
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

void LDA_immediate_test() 
{
    mem[0] = 0xa9;
    mem[1] = 0xFF;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;
    
    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 2);

    exit(assert(cpu.A == 0xFF));
}

void LDA_neg_flag_test()
{
    mem[0] = 0xa9;
    mem[1] = 0xFF;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 2);

    exit(assert(cpu.N == 1));
}

// zero flag test
void LDA_z_flag_test()
{
    mem[0] = 0xa9;
    mem[1] = 0x00;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 2);
    
    exit(assert(cpu.Z == 1));
}

void STA_test()
{
    mem[0] = 0xa9;
    mem[1] = 0x69; // nice
    mem[2] = 0x85;
    mem[3] = 0x00;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem, &on_system_tick);


    execute(&cpu, 5);
   
    exit(assert(mem[0] == 0x69));
}

void LDA_zp_test() 
{
    mem[0] = 0xa5;
    mem[1] = 0x00;
    mem[0xFFFC] = 0x00;
    mem[0xFFFD] = 0x00;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 3);

    int rval = 0;

    rval += assert(cpu.A == 0xa5);
    rval += assert(cpu.N == 1);
    rval += assert(cpu.Z == 0);

    exit(rval);
}

void LDA_zp_X_test() 
{
    mem[0] = 0xa2;
    mem[1] = 1;
    mem[2] = 0xb5;
    mem[3] = 1;
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 6);
    
    int rval = 0;

    rval += assert(cpu.A == 0xb5);
    rval += assert(cpu.N == 1);
    rval += assert(cpu.Z == 0);

    exit(rval);
}

void LDX_immediate_test()
{
    mem[0] = 0xa2;
    mem[1] = 1;
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 2);

    exit(assert(cpu.X == 1));

}
