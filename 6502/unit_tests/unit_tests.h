#ifndef _UNIT_TESTS_H
#define _UNIT_TESTS_H

#include <stdio.h>
#include "../include/6502.h"

#define assert( exp ) \
    ( (exp) ? assert_passed(__LINE__, __FILE__) : assert_failed( __LINE__, __FILE__))

int assert_failed(int line, char *file);
int assert_passed(int line, char *file);

void LDA_immediate_test();
void LDA_neg_flag_test();
void LDA_z_flag_test();
void STA_test();
void LDA_zp_test();
void LDA_zp_X_test();
void LDX_immediate_test();

#endif
