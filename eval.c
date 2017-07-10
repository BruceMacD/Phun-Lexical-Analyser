/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

operation* stack[MAXSIZE];
int pos = -1;

operation *newOperation (identifierType type, operation *next, int result) {
    operation *o = malloc(sizeof (operation));
    o->type = type;
    o->nextOperation = next;
    o->result = result;
    return (o);
}

/*
 * AST Dump for evaluation
 */

void printExpr(expr *e, int n) {
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
        case '+':
            //push operation to stack
            push(newOperation(oADD, NULL, 0));
            break;
        case '-':
            //set operation to subtraction
            //push operation to stack
            push(newOperation(oSUB, NULL, 0));
            break;
        case '*':
            //push operation to stack
            push(newOperation(oDIV, NULL, 0));
            break;
        case '/':
            //push operation to stack
            push(newOperation(oMULT, NULL, 0));
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
        performOperation(stack[pos+1]->result);
    }
}

void push(operation *op) {
    if (pos != MAXSIZE) {
        pos = pos + 1;
        stack[pos] = op;
    } else {
        fatalError ("Stack overflow");
    }

}

void performOperation(int value) {
    //peek the stack
    operation *op = stack[pos];
    if (op->result == NULL) {
        //set first value to operate on
        op->result = value;
    } else {
        switch (op->type) {
            case oADD:
                //add to value
                printf("%d + %d\n", stack[pos]->result, value);
                op->result = op->result + value;
                break;
            case oSUB:
                //sub from value
                printf("%d - %d\n", stack[pos]->result, value);
                op->result = op->result - value;
                break;
            case oMULT:
                //multiply value
                op->result = op->result * value;
                break;
            case oDIV:
                //divide value
                op->result = op->result * value;
                break;
            default:
                //check defined identifiers
                break;
        }
    }
}

int evalList(exprs *l, int n) {
    if (l == NULL) return -1;
    printExpr(l->e, n);
    evalList(l->n, n);
    //return end result from last value in stack
    return stack[0]->result;
}

/* end of eval.c */
