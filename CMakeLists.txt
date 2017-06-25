cmake_minimum_required(VERSION 3.7)
project(Phun_Lexical_Analyser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lexer.c
        main.c
        phun.h parser.c)

add_executable(Phun_Lexical_Analyser ${SOURCE_FILES})