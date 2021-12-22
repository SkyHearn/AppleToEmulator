add_executable(unit_tests
               "${PROJECT_SOURCE_DIR}/include/6502.h"
               unit_tests.h
               unit_tests.c)

target_link_libraries(unit_tests lib6502)

target_include_directories(unit_tests PUBLIC 
                           "${PROJECT_BINARY_DIR}" 
                           "${PROJECT_SOURCE_DIR}/6502")

add_test(NAME LDA_immidiate_test 
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests" 
         "0")

add_test(NAME LDA_neg_flag_test
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "1")

add_test(NAME LDA_z_flag_test
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "2")

add_test(NAME STA_test
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "3")

add_test(NAME LDA_ZP
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "4")

add_test(NAME LDA_ZP_X
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "5")

add_test(NAME LDX_immidiate_test
         COMMAND "${PROJECT_BINARY_DIR}/unit_tests/unit_tests"
         "6")
