cmake_minimum_required (VERSION 3.12)
project(6502_simulator)

add_subdirectory("unit_tests")

add_library(lib6502
    STATIC
        include/6502.h
        6502-internal.h
	6502.c
)
