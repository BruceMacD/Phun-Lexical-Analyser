/* 
 * Phun Lexical Analyzer
 * Rebecca Ansems, Brian Bremner, Bruce MacDonald, June 2017
 *
 * takes a single command line parameter, the name of a file to analyse
 * prints out the tokens it finds
 *
 * Built on provided sample code from: http://www.tamimeredith.ca/downloads/csci3136/lexer.c
 */

/* 
 * Our grammar:
 *
 * Tokens:
 * (, ), [0-9], "strings", identifiers (upper case, lower case, extended chars), eof
 *
 * whitespace, new line, and eof end a token
 *
 * no token can be longer than 1023 characters in length
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SUCCESS  0
#define FAILURE -1

/*
 * Types for lexical analysis
 */
typedef enum { sSTART, sN, sNE, sINT } state;
typedef enum { tPLUS, tMINUS, tTIMES, tDIVIDE, tNEG, tINT, tEOF } tokentype;
typedef struct {
    tokentype type;
    int       value;
} token;

/*
 * Global variables for input file
 */
FILE *ifp;
char *name;
int   idx = 0;

/*
 * Basic error function. Print a message and abort.
 */
void fatalError (char *msg) {
    printf("Fatal Error: %s: %d: %s. Aborting.\n", name, idx, msg);
    exit(FAILURE);
}

/*
 * Read one character at a time from the input file
 * and update the index for error messages
 */
char nextChar () {
    char c = fgetc (ifp);
    idx++;
#if DEBUG
    printf("Read character %d: %c\n", idx, c);
#endif
    return(c);
}

/* 
 * Return a character to the input stream 
 */
void returnChar (char c) {
    ungetc(c, ifp);
    idx--;
}

/*
 * Used for debugging only.
 */
void printToken (token t) {
    printf("Token: ");
    switch (t.type) {
        case tPLUS: printf("+"); break;
        case tMINUS: printf("-"); break;
        case tTIMES: printf("*"); break;
        case tDIVIDE: printf("/"); break;
        case tNEG: printf("negation operator"); break;
        case tINT: printf("Integer %d", t.value); break;
        case tEOF: printf("End of File"); break;
        default: break;
    }
    printf("\n");
}

/*
 * The basic lexical analyser (a simple DFSA)
 */
token scan() {

    char c;
    state s = sSTART;
    token t;
    int   value = 0;

    while (c = nextChar()) {

        switch (s) {
            case sSTART:
                if (isspace(c)) break;
                switch (c) {
                    case EOF: t.type = tEOF;    return(t);
                    case '+': t.type = tPLUS;   return(t);
                    case '-': t.type = tMINUS;  return(t);
                    case '*': t.type = tTIMES;  return(t);
                    case '/': t.type = tDIVIDE; return(t);
                    case 'n': s = sN; break;
                    default:
                        if (isdigit(c)) {
                            value = c - '0';
                            s = sINT;
                        } else{
                            fatalError("Invalid character X");
                        }
                        break;
                }
                break;
            case sN:
                if (c == 'e')
                    s = sNE;
                else
                    fatalError("Invalid token");
                break;
            case sNE:
                if (c == 'g') {
                    t.type = tNEG;
                    return(t);
                }
                else
                    fatalError("Invalid token");
                break;
            case sINT:
                if (isdigit(c)) {
                    value = (value * 10) + (c - '0');
                } else if (isspace(c)) {
                    t.value = value;
                    t.type = tINT;
                    return(t);
                } else {
                    returnChar(c);
                }
                break;
            default:
                if (c == EOF) t.type = tEOF; return(t);
                if (!isspace(c))
                    fatalError ("Invalid character Y");
                break;
        }
    }
}

/*
 * Temporary main. We'll replace this when we add the parser.
 */
int main (int argc, char** argv) {

    token t;
    ifp = fopen (argv[1], "r");
    name = argv[1];
    while (1) {
        t = scan();
#if DEBUG
        printToken(t);
#endif
        if (t.type == tEOF) break;
    }
    return (SUCCESS);
}