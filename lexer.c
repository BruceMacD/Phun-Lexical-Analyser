/* 
 * Phun Lexical Analyzer
 * Rebecca Ansems, Brian Bremner, Bruce MacDonald, June 2017
 *
 * takes a single command line parameter, the name of a file to analyse
 * prints out the tokens it finds
 *
 * Built on provided sample code from: http://www.tamimeredith.ca/downloads/csci3136/lexer.c
 * Must be built using gcc for case ranges
 */

/* 
 * Our grammar:
 *
 * Tokens:
 * (), [0-9], "strings", identifiers (upper case, lower case, extended chars), eof
 *
 * whitespace, new line, and eof end a token
 *
 * no token can be longer than 1023 characters in length
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SUCCESS  0
#define FAILURE -1

/*
 * Types for lexical analysis
 */
typedef enum { sSTART, sSTRING, sIDENTIFIER, sINT } state;
typedef enum { tPUNCTUATION, tINT, tSTRING, tIDENTIFIER, tEOF } tokentype;
typedef struct {
    //stores type, int value, or String
    tokentype type;
    int       value;
    //TODO: refactor this? No token can be longer than 1023
    char    characters[1023];
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
 */
char nextChar () {
    char c = fgetc (ifp);
    idx++;
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
    switch (t.type) {
        case tPUNCTUATION: printf("(%s, %s)", "Punctuation", t.characters); break;
        case tINT: printf("(%s, %d)", "Integer", t.value); break;
        case tSTRING: printf("(%s, %s)", "String", t.characters); break;
        case tIDENTIFIER: printf("(%s, %s)", "Identifier", t.characters); break;
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
    int   charIndex = 0;
    //char  characters[1023];

    while (c = nextChar()) {

        switch (s) {
            case sSTART:
                if (isspace(c)) break;
                //set the value of the token
                strcpy(t.characters, &c);
                switch (c) {
                    case EOF: t.type = tEOF;    return(t);
                    //assumes () are separate and not enclosing
                    case '(': t.type = tPUNCTUATION;  return(t);
                    case ')': t.type = tPUNCTUATION;  return(t);
                    //set the value to append to if string
                    //case '"': s = sSTRING;            strcpy(characters, &c); break;
                    //identifier range
                    case '!': t.type = tIDENTIFIER;   return(t);
                    case '$': t.type = tIDENTIFIER;   return(t);
                    case '%': t.type = tIDENTIFIER;   return(t);
                    case '&': t.type = tIDENTIFIER;   return(t);
                    case '*': t.type = tIDENTIFIER;   return(t);
                    case '/': t.type = tIDENTIFIER;   return(t);
                    case ':': t.type = tIDENTIFIER;   return(t);
                    case '<': t.type = tIDENTIFIER;   return(t);
                    case '=': t.type = tIDENTIFIER;   return(t);
                    case '>': t.type = tIDENTIFIER;   return(t);
                    case '?': t.type = tIDENTIFIER;   return(t);
                    case '^': t.type = tIDENTIFIER;   return(t);
                    case '_': t.type = tIDENTIFIER;   return(t);
                    case '~': t.type = tIDENTIFIER;   return(t);
                    case '+': t.type = tIDENTIFIER;   return(t);
                    case '-': t.type = tIDENTIFIER;   return(t);
                    //check uppercase ASCII range
                    case 65 ... 90: t.type = tIDENTIFIER;   return(t);
                    //check lowercase ASCII range
                    case 97 ... 122: t.type = tIDENTIFIER;   return(t);
                    default:
                        if (isdigit(c)) {
                            // catch the integer sequences
                            value = c - '0';
                            s = sINT;
                        } else if (c == '"') {
                            //catch string sequences
                            t.characters[charIndex] = c;
                            s = sSTRING;
                        }
                        else {
                            printf("Fatal Error: %d is an invalid character\n", c);
                            fatalError("Aborting.");
                        }
                        break;
                }
                break;
            //capture a sequence of integers
            case sINT:
                if (c == EOF){
                    t.value = value;
                    t.type = tINT;
                    return(t);
                }
                else if (isdigit(c)) {
                    value = (value * 10) + (c - '0');
                } else if (isspace(c)) {
                    t.value = value;
                    t.type = tINT;
                    return(t);
                } else {
                    returnChar(c);
                }
                break;
            //capture a string sequence until a closing quote, up to 1023
            case sSTRING:
                //append to current characters
                charIndex++;
                if (charIndex < 1023) {
                    t.characters[charIndex] = c;
                }
                if (c == '"') {
                    t.type = tSTRING;
                    return(t);
                }
                break;
            case sIDENTIFIER:
                //TODO: After the first character, identifers can also contain digits, as well as any of the...
                //TODO: ... allowable initial characters
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
    //shows format to user
    printf("(type, value)\n");
    while (1) {
        t = scan();
        printToken(t);
        if (t.type == tEOF) break;
    }
    return (SUCCESS);
}
