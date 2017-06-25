/* 
 * Simple Expression Interpreter
 * -- MAINLINE & Utilities --
 * Tami Meredith, June 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

/*
 * Global variables for input file
 */
FILE *ifp;
char *name;
int   idx = 0;
int   line = 1; 

/*
 * Basic error function. Print a message and abort.
 */
void fatalError (char *msg) {
    printf("Fatal Error: %s: %d (%d): %s. Aborting.\n", name, line, idx, msg);
    exit(FAILURE);
}

/*
 * Read one character at a time from the input file
 * and update the index for error messages
 */
char nextChar () {
   char c = fgetc (ifp);
   idx++;
   if (c == '\n') line++;
#if DEBUG
   // printf("Read character %d: %c\n", idx, c);
#endif
   return(c);
}

/* 
 * Return a character to the input stream 
 */
void returnChar (char c) {
    ungetc(c, ifp);
    idx--;
    if (c == '\n') line--;
}

/*
 * Temporary main. We'll replace this when we add the AST.
 */
int main (int argc, char** argv) {

    ifp = fopen (argv[1], "r");
    name = argv[1];

    //create and parse AST
    evaluate(parse());

    return (SUCCESS);
}

/* end of main.c */
