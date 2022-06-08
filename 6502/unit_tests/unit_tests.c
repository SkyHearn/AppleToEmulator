#include "unit_tests.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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

void load_bin(char *fname) {    
    FILE *fp = fopen(fname, "r");

    if(fp == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    fread(mem, sizeof(uint8_t), 0xFFFE, fp);

    fclose(fp); 
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
            STA_zp_test();
            break;
        case 2:
            LDA_zp_test();
            break;
        case 3:
            LDA_zp_X_test();
            break;
        case 4:
            LDX_immediate_test();
            break;
        case 5:
            LDA_abs_test();
            break;
        case 6:
            ADC_imm_test();
            break;
        case 7:
            ADC_zp_test();
            break;
        case 8:
            ADC_zp_X_test();
            break;
        case 9:
            ADC_abs_test();
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
    load_bin("lda_imm.bin");    
    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 2);
    
    bool status = (cpu.A == 0xFF) && (cpu.N == 1) && (cpu.Z == 0);

    exit(assert(status));
}

void STA_zp_test()
{
    load_bin("sta_zp.bin");
    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 5);
   
    exit(assert(mem[0] == 0x69));
}

void LDA_zp_test() 
{
    load_bin("lda_zp.bin");

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 3);

    int rval = 0;

    rval += assert(cpu.A == 0x69);
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

void LDA_abs_test() 
{
    mem[0] = 0xad;
    mem[1] = 0xFF;
    mem[2] = 0xFF;
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;
    mem[0xFFFF] = 69;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 4);

    exit(assert(cpu.A == 69));
}

void ADC_imm_test() 
{
    // set accumulator to 1
    mem[0] = 0xa9;
    mem[1] = 0x01;
    // add 2 to the accumulator
    mem[2] = 0x69;
    mem[3] = 0x02;
    // store accumulator
    mem[4] = 0x85;
    mem[5] = 0x00;
    // reset vector
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 7);

    exit(assert(mem[0] == 0x03));
}

void ADC_zp_test()
{
    // set accumulator to 1
    mem[0] = 0xa9;
    mem[1] = 0x01;
    // add the value stored at byte 
    // 0x06 to the accumulator
    mem[2] = 0x65;
    mem[3] = 0x06;
    // store accumulator
    mem[4] = 0x85;
    mem[5] = 0x00;
    // value stored at 0x06
    mem[0x06] = 0x02;
    // reset vector
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 8);

    exit(assert(mem[0] == 0x03));
}

void ADC_zp_X_test() 
{
    // set accumulator to 1
    mem[0] = 0xa9;
    mem[1] = 0x01;
    // set the X register to 2
    mem[2] = 0xa2;
    mem[4] = 0x02;
    // add the value stored at byte
    // 0x08 + X
    mem[5] = 0x75;
    mem[6] = 0x0e;
    // store the accumulator
    mem[7] = 0x85;
    mem[8] = 0x00;
    // *(0x0e + X) = 2
    mem[0x10] = 0x2;
    // reset vector
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 11);

    exit(assert(mem[0] == 0x03));
}

void ADC_abs_test()
{
    // set accumulator to 1
    mem[0] = 0xa9;
    mem[1] = 0x01;
    // add the value stored at address 
    // 0x0007 to the accumulator
    mem[2] = 0x6D;
    mem[3] = 0x07;
    mem[4] = 0x00;
    // store the accumulator
    mem[5] = 0x85;
    mem[6] = 0x00;
    // value stored at 0x0007
    mem[0x07] = 0x02;
    // reset vector
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 8);

    exit(assert(mem[0] == 0x03));
}

// not done
void ADC_abs_x_test()
{
    
    // set accumulator to 1
    mem[0] = 0xa9;
    mem[1] = 0x01;
    // set X to 3
    mem[2] = 0xa2;
    mem[3] = 0x03;
    // add the value stored at address 
    // 0x0006 + X to the accumulator
    mem[4] = 0x7d;
    mem[5] = 0x06;
    mem[6] = 0x00;
    // store the accumulator
    mem[7] = 0x85;
    mem[8] = 0x00;
    // value stored at 0x0006 + X
    mem[0x09] = 0x02;
    // reset vector
    mem[0xFFFC] = 0;
    mem[0xFFFD] = 0;

    CPU cpu = initialize_cpu(mem, &on_system_tick);

    execute(&cpu, 8);

    exit(assert(mem[0] == 0x03));
}