cmake_minimum_required(VERSION 3.27)
project(CDataX C)

set(CMAKE_C_STANDARD 11)

add_executable(CDataX
        main.c
        column.c
        column.h
        llc.c
        llc.h
        cdataframe.c
        cdataframe.h
)
