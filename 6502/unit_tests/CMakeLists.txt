add_executable(unit_tests
               "${PROJECT_SOURCE_DIR}/include/6502.h"
               unit_tests.h
               unit_tests.c)

target_link_libraries(unit_tests 6502)

target_include_directories(unit_tests PUBLIC 
                           "${PROJECT_BINARY_DIR}" 
                           "${PROJECT_SOURCE_DIR}/6502")

add_test(NAME LDA_immidiate_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests" 
         "0")

add_test(NAME STA_ZP
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "1")

add_test(NAME LDA_ZP
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "2")

add_test(NAME LDA_ZP_X
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "3")

add_test(NAME LDX_immidiate_test
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "4")

add_test(NAME LDA_absolute_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "5")
add_test(NAME ADC_imm_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "6")
add_test(NAME ADC_zp_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "7")
add_test(NAME ADC_zp_X_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "8")
add_test(NAME ADC_abs_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "9")