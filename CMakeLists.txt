cmake_minimum_required(VERSION 3.7)
project(Phun_Lexical_Analyser)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES lexer.c)
add_executable(Phun_Lexical_Analyser ${SOURCE_FILES})