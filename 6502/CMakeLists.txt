cmake_minimum_required (VERSION 3.12)
project(6502)

add_subdirectory("unit_tests")

add_library(6502
    STATIC
        6502.h
	    6502.cpp
)