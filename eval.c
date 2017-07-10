/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"


/*
 * AST Dump for debugging and testing
 */

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
            evalList(e->eVal, n+1);
            indent(n);
            printf(")\n");
            break;
        default:
            break;
    }
}

void evalList(exprs *l, int n) {
    if (l == NULL) return;
    printExpr(l->e, n);
    evalList(l->n, n);
}

/* end of eval.c */
