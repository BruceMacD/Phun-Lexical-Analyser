/* 
 * Simple Expression Interpreter
 * -- MAINLINE & Utilities --
 * Tami Meredith, June 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

/* STAGE == 1:SCAN, 2:PARSE, 3:EVAL */
#define STAGE 3

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
 * Temporary main. We'll replace this when we add the parser.
 */
int main (int argc, char** argv) {

#if STAGE == 1 /* Scan */
    token t;
#else /* Parse or Evaluate */
    exprs *e;
#endif
    ifp = fopen (argv[1], "r");
    name = argv[1];

#if STAGE == 1 /* Scan */
    while (1) {
        t = scan();
        printToken(t);
        if (t.type == tEOF) break;
    }
#elif STAGE == 2 /* Parse */
    e = parse();
    //print the list
    //printList(e, 0);
#else /* Evaluate */
    e = parse();
    //evaluate tree
    evalList(e, 0);
    /* Evaluate e here */
#endif
    return (SUCCESS);
}

/* end of main.c */