cmake_minimum_required(VERSION 3.7)
project(Phun_Lexical_Analyser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        phun.h
        eval.c
        lexer.c
        main.c
        parser.c)

add_executable(Phun_Lexical_Analyser ${SOURCE_FILES})