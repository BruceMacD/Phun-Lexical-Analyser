/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

atom* stack[MAXSIZE];
int pos = -1;

atom *newAtom (identifierType type, atom *next, struct expression* result) {
    atom *a = malloc(sizeof (atom));
    a->type = type;
    a->nextAtom = next;
    a->result = result;
    return (a);
}


/*
 * Atom expression Functions
 */

expr *newListResult(exprs *l) {
    expr *result = malloc(sizeof (expr));
    result->eVal = l;
    result->type = eExprList;
    return (result);
}

expr *newIntResult(int i) {
    expr *result = malloc(sizeof (expr));
    result->iVal = i;
    result->type = eInt;
    return (result);
}

/*
 * AST Dump for evaluation
 */

void evalExpr(expr *e, int n) {
    switch (e->type) {
        case eString:
            printf("String: [%s]\n", e->sVal);
            break;
        case eIdent:
            //identify in symbol table
            symbolTable(e->sVal);
            break;
        case eInt:
            //perform operation
            performOperation(e->iVal);
            break;
        case eExprList:
            evalList(e->eVal, n+1);
            pop();
            break;
        default:
            break;
    }
}

void symbolTable(char *sVal) {

    //check for definition
    if (strcmp(sVal, "define") == 0) {
        //add defined value to symbol table
        printf("define");
    }

    //TODO: car, cat, etc

    switch (*sVal) {
        case '"':
            //strip quote and return atom
        case '+':
            //push operation to stack
            push(newAtom(oADD, NULL, NULL));
            break;
        case '-':
            //set operation to subtraction
            //push operation to stack
            push(newAtom(oSUB, NULL, NULL));
            break;
        case '*':
            //push operation to stack
            push(newAtom(oMULT, NULL, NULL));
            break;
        case '/':
            //push operation to stack
            push(newAtom(oDIV, NULL, NULL));
            break;
        default:
            //check defined identifiers
            break;
    }
}

void pop() {
    if(pos != 0) {
        //don't pop last value
        pos = pos - 1;
        //set the result in the current op from final value
        performOperation(stack[pos+1]->result->iVal);
    }
}

void push(atom *at) {
    if (pos != MAXSIZE) {
        pos = pos + 1;
        stack[pos] = at;
    } else {
        fatalError ("Stack overflow");
    }

}

void performOperation(int value) {
    //peek the stack
    atom *at = stack[pos];
    if (at->result == NULL) {
        //set first value to operate on
        at->result = newIntResult(value);
    } else {
        switch (at->type) {
            case oADD:
                //add to value
                printf("%d + %d\n", stack[pos]->result->iVal, value);
                at->result->iVal = at->result->iVal + value;
                break;
            case oSUB:
                //sub from value
                printf("%d - %d\n", stack[pos]->result->iVal, value);
                at->result->iVal = at->result->iVal - value;
                break;
            case oMULT:
                //multiply value
                at->result->iVal = at->result->iVal * value;
                break;
            case oDIV:
                //divide value
                at->result->iVal = at->result->iVal / value;
                break;
            default:
                //check defined identifiers
                break;
        }
    }
}

atom* evalList(exprs *l, int n) {
    if (l == NULL) return NULL;
    evalExpr(l->e, n);
    evalList(l->n, n);
    //return end result from last value in stack
    return stack[0];
}

/* end of eval.c */
