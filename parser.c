/* 
 * Phun Interpreter
 * Parser Code (including AST creation)
 * Tami Meredith, June 2017
 */

/*
 * The Grammar:
 * FILE  := EXPRS
 * EXPRS := EXPR EXPRS
 *       |  epsilon
 * EXPR  := ident | int | string
 *       |  ( EXPRS )
 *       |  ' EXPR
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "phun.h"

/* 
 * AST Construction Functions 
 */ 
expr *newStringExpr(char *s) {
    expr *result = malloc(sizeof (expr));
    result->sVal = s;
    result->type = eString;
    return (result);
}

expr *newIdentExpr(char *s) {
    expr *result = malloc(sizeof (expr));
    result->sVal = s;
    result->type = eIdent;
    return (result);
}

expr *newListExpr(exprs *l) {
    expr *result = malloc(sizeof (expr));
    result->eVal = l;
    result->type = eExprList;
    return (result);
}

expr *newIntExpr(int i) {
    expr *result = malloc(sizeof (expr));
    result->iVal = i;
    result->type = eInt;
    return (result);
}

exprs *newExprList(expr *e, exprs *n) {
    exprs *result = malloc(sizeof (exprs));
    result->e = e;
    result->n = n;
    return (result);
}

/*
 * AST Dump for debugging and testing
 */
void printList(exprs *l, int n); 

void indent(int n) {
    int i;
    for (i = 0; i < n; i++)
        printf("  ");
}
  
void printExpr(expr *e, int n) {
    indent(n);
    switch (e->type) {
        case eString:
            printf("String: [%s]\n", e->sVal);
            break;
        case eIdent:
            printf("Identifier: [%s]\n", e->sVal);
            break;
        case eInt:
            printf("Integer: [%d]\n", e->iVal);
            break;
        case eExprList:
            printf("(\n");
            printList(e->eVal, n+1);
            indent(n);
            printf(")\n");
            break;
        default:
            break;
    }
}
 
void printList(exprs *l, int n) {
    if (l == NULL) return;
    printExpr(l->e, n);
    printList(l->n, n);
}
 
/*
 * Parsing Rules
 */ 
exprs *parseFileList (token t) {
    expr *e;
    if (t.type == tEOF) {
        return (NULL);
    } else {
        e = parseExpr(t);
        return (newExprList(e, parseFileList(scan())));
    }
}

exprs *parseExprList (token t) {
    expr *e;
    if (t.type == tEND) {
        return (NULL);
    } else {
        e = parseExpr(t);
        return (newExprList(e, parseExprList(scan())));
    }
}

expr *parseExpr (token t) {
    switch (t.type) {
        case tBEGIN:
            return (newListExpr (parseExprList (scan())));
        case tINT:
            return (newIntExpr (t.iVal));
        case tIDENT:
            return (newIdentExpr (t.sVal));
        case tSTRING:
            return (newStringExpr (t.sVal));
        case tQUOTE:
            return  (newListExpr
                        (newExprList 
                            (newIdentExpr("quote"),
                                newExprList(parseExpr(scan ()), NULL))));
        default:
            /* Oh noes, something went awry! */
            fatalError ("Syntax Error");
    }
}

exprs *parse () {
    return(parseFileList(scan()));
}

/* end of parser.c */