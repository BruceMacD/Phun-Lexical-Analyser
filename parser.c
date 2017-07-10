/*
 * Simple Expression Interpreter
 * AST creation and evaluation
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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