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
//store first defined identifier in linked list
identifier* iHEAD = NULL;
//for navigating list of defined identifiers
identifier* iCURR;
//for returning result
atom* result = NULL;

atom *newAtom (identifierType type, atom *next, struct expression* result) {
    atom *a = malloc(sizeof (atom));
    a->type = type;
    a->nextAtom = next;
    a->result = result;
    return (a);
}

identifier *newIdentifier(char* name, int data, identifier *next) {
    identifier *i = malloc(sizeof (i));
    i->name = name;
    i->data = data;
    i->next = next;
    return (i);
}
/*
 * expression Functions
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

    //in definition state, add to global symbol table
    //check for definition
    if (pos != -1 && stack[pos]->type == oDEFINE) {
        //get current
        setCurrentIdentifier();
        iCURR->name = sVal;
    }
    else if (strcmp(sVal, "define") == 0) {
        //add defined value to symbol table
        push(newAtom(oDEFINE, NULL, NULL));
        if (iHEAD == NULL) {
            //set the head
            iHEAD = newIdentifier(NULL, NULL, NULL);
        } else {
            //set the next identifier
            setCurrentIdentifier();
            iCURR->next = newIdentifier(NULL, NULL, NULL);
        }
    }
    else if (strcmp(sVal, "quote") == 0) {
        //strip quote and return atom
        printf("quote");
        fflush(stdout);
    }
    else {
        //TODO: car, cat, etc
        switch (*sVal) {
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
                if (iHEAD != NULL) {
                    iCURR = iHEAD;
                    if (strcmp(sVal, iCURR->name) == 0) {
                        //identifier found
                        performOperation(iCURR->data);
                    }
                    else {
                        while (iCURR != NULL) {
                            if (strcmp(sVal, iCURR->name) == 0) {
                                //identifier found
                                performOperation(iCURR->data);
                                break;
                            }
                            iCURR = iCURR->next;
                        }
                    }
                }
                break;
        }
    }
}

void pop() {
    if(pos >= 0) {
        result = stack[pos];
        pos = pos - 1;
        //set the result in the current op from final value
        //TODO: Will need to change this for lists
        if(pos >= 0) {
            performOperation(result->result->iVal);
        }
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
    if (at->type == oDEFINE) {
        //set the value of the custom identifier
        setCurrentIdentifier();
        iCURR->data = value;
    }
    else if (at->result == NULL) {
        //switch (at->type) {
            //TODO: might be able to get rid of this
            //default:
                //add, sub, mult, divide
                //set first value to operate on
                at->result = newIntResult(value);
                //break;
        //}
    } else {
        switch (at->type) {
            case oDEFINE:
                at->result->iVal = value;
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
                fatalError ("Unrecognized operation");
                break;
        }
    }
}

void setCurrentIdentifier() {
    //find the last custom identifier
    iCURR = iHEAD;
    while (iCURR->next != NULL) {
        iCURR = iCURR->next;
    }
}

atom* evalList(exprs *l, int n) {
    if (l == NULL) return NULL;
    evalExpr(l->e, n);
    evalList(l->n, n);
    //return end result from last value in stack
    return result;
}

/* end of eval.c */
