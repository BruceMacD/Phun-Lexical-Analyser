/* 
 * Phun Interpreter
 * Lexical Analysis Code
 * Tami Meredith, June 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"
 
/* Length of the longest identifier or string must be < BUFLEN */
#define BUFLEN 1024

/*
 * Used for debugging only.
 */
void printToken (token t) {
   printf("Token: ");
   switch (t.type) {
      case tBEGIN: printf("("); break;
      case tEND: printf(")"); break;
      case tIDENT: printf("Identifier [%s]", t.sVal); break;
      case tSTRING: printf("String [%s]", t.sVal); break;
      case tQUOTE: printf("Quote"); break;
      case tINT: printf("Integer [%d]", t.iVal); break;
      case tEOF: printf("End of File"); break;
      default: break;
    } 
    printf("\n");
}

/* 
 * Extended characters allowed in identifiers 
 */
int isextended(char c) {
    switch(c) {
        case '!':
        case '$':
        case '%':
        case '&':
        case '*':
        case '/':
        case ':':
        case '<':
        case '=':
        case '>':
        case '?':
        case '^':
        case '_':
        case '~':
        case '+':
        case '-':
            return(1);
        default:
            return(0);
    }
}

/*
 * The basic lexical analyser (a simple DFSA)
 */
token scan() {

    state s = sSTART;
    token t;
    char  c;
    int   value = 0;
    char  buf[BUFLEN];
    int   len = 0;
    
    while (c = nextChar()) {

        switch (s) {

            case sSTART:
                if (isspace(c)) break;
                switch (c) {
                    case ';':
                        s = sCOMMENT;
                        break; 
                    case ')': 
                        t.type = tEND;    
                        return(t);
                    case '(': 
                        t.type = tBEGIN;    
                        return(t);
                    case EOF: 
                        t.type = tEOF;    
                        return(t);
                    case '\'':
                        t.type = tQUOTE;
                        return(t);
                    case '"': 
                        s = sSTRING;
                        break;    
                    default:
                        if (isdigit(c)) {
                            value = c - '0';
                            s = sINT;
                        } else if (isalpha(c) || isextended(c)) {
                            buf[len++] = c;
                            s = sIDENT;
                        } else {
                            fatalError("Invalid character");
                        }
                        break; 
                }
                break;

            case sCOMMENT:
                if (c == '\n') 
                    s = sSTART;
                break;
         
            case sINT:
                if (isdigit(c)) {
                    value = (value * 10) + (c - '0');
                } else if (isspace(c)) {
                    t.iVal = value;
                    t.type = tINT;
                    return(t);
                } else {
                    returnChar(c);
                    t.iVal = value;
                    t.type = tINT;
                    return(t);
                } 
                break;
 
            case sSTRING:
                if (c == '\n') {
                    fatalError("End of line in a string");
                } else if (c == EOF) {
                    fatalError("End of file in a string");
                } else if (c == '"') {
                    buf[len] = 0;
                    t.sVal = strdup(buf);
                    t.type = tSTRING;
                    return(t);
                } else {
                    buf[len++] = c;
                    if (len >= BUFLEN) 
                        fatalError("String too long");
                } 
                break;

            case sIDENT:
                if (isalpha(c) || isextended(c) || isdigit(c)) {
                    buf[len++] = c;
                    if (len >= BUFLEN) 
                        fatalError("Identifier too long");
                } else if (isspace(c)) {
                    buf[len] = 0;
                    t.sVal = strdup(buf);
                    t.type = tIDENT;
                    return(t);
                } else {
                    returnChar(c);
                    buf[len] = 0;
                    t.sVal = strdup(buf);
                    t.type = tIDENT;
                    return(t);
                } 
                break;

            default:
                /* This should be dead code */
                if (c == EOF) t.type = tEOF; return(t);
                if (!isspace(c))
                    fatalError ("Invalid character");
                break;
        }        
    }
}

/* end of lexer.c */