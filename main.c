/* 
 * Simple Expression Interpreter
 * -- MAINLINE & Utilities --
 * Tami Meredith, June 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#if STAGE == 3 /* Evaluation - need a symbol table */
    symtab st;
    //function table
    funtab ft;
#endif

/*
 * Basic error function. Print a message and abort.
 */
void fatalError (char *msg) {
    printf("Fatal Error: %s: %d (%d): %s. Aborting.\n", name, line, idx, msg);
    exit(FAILURE);
}

void evalError (char *name) {
    printf("Evaluation Error: Unbound function %s\n", name);
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
 * Find an identifier in the symbol table
 * - simple linear search (good enough for now)
 */
symbol *lookup(char *name) {
    symbol *s;
    if (st.length == 0) return NULL;
    s = st.first;
    while (s != NULL) {
        if (!strcmp(name,s->name)) return(s);
        s = s->next;
    }
    return NULL;
}

/* 
 * Add an identifier to the symbol table
 */
symbol *bind(char *name, expr *val) {
    symbol *s = malloc (sizeof (symbol));
    s->name = name;
    s->data = val;
    s->next = st.first;
    st.first = s;
    st.length++;
    return(s);
}

/*
 * Find a function in the function table
 */
function *lookupFunction(char *name) {
    function *f;
    if (ft.length == 0) return NULL;
    f = ft.first;
    while (f != NULL) {
        if (!strcmp(name,f->name)) return(f);
        f = f->next;
    }
    return NULL;
}

/*
 * Add an identifier to the function table
 */
function *bindFunction(char *name, expr *val) {
    function *f = malloc (sizeof (function));
    //function symbol table for needed identifiers
    symtab fst;
    fst.length = 0;
    fst.first = NULL;
    f->name = name;
    f->operation = val;
    //set the val to after the lambda
    struct exprList *exprL = val->eVal->n->e->eVal;
    //create bindings for the symbols attached to the function
    while (exprL != NULL) {
        if (exprL->e->type == eIdent) {
            //add to local symbol table
            symbol *s = malloc (sizeof (symbol));
            s->name = exprL->e->sVal;
            //no data yet, will be assigned on call
            s->data = NULL;
            s->next = fst.first;
            fst.first = s;
            fst.length++;
        }
        //go to the next value
        exprL = exprL->n;
    }
    f->fst = fst;
    //set the operation to the expression after the identifier
    f->operation = val->eVal->n->n->e;
    f->next = ft.first;
    ft.first = f;
    ft.length++;
    return(f);
}

/*
 * Mainline for interpreter (all versions)
 */
int main (int argc, char** argv) {

#if STAGE == 1 /* Scan */
    token t;
#else /* Parse or Evaluate */
    exprs *e;
    expr  *result;
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
    printList(e, 0);
#else /* Evaluate */
    /* initialise an empty global symbol table */
    st.length = 0;
    st.first = NULL;
    
    e = parse();
    while (e != NULL) {
        exprPrint(e->e);
        printf("\n");
        result = eval(e->e);
        if (result != NULL) {
            printf("=> ");
            exprPrint(result);
            printf("\n");
        }
        e = e->n;
    }
#endif
    return (SUCCESS);
}

/* end of main.c */