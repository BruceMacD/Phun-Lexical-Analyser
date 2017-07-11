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
//store list
identifier* iList = NULL;
//for returning result
atom* result = NULL;

atom *newAtom (identifierType type, int iVal, identifier* listHead) {
    atom *a = malloc(sizeof (atom));
    a->type = type;
    //running result of operations
    a->iVal = iVal;
    //pointer to head of list attached to atom
    a->listHead = listHead;
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
            // do not evaluate empty list
            if (e->eVal != NULL) {
                evalList(e->eVal, n+1);
                pop();
            }
            break;
        default:
            break;
    }
}

void symbolTable(char *sVal) {
    //TODO: car, cdr, cons
    //in definition state, add to global symbol table
    //check for definition
    if (pos != -1 && stack[pos]->type == oDEFINE) {
        //check if identifier is already defined and remove the old value
        removeIdentifier(sVal);
        //set the new identifier at the end of the list
        setCurrentIdentifier();
        iCURR->name = sVal;
    }
    else if (strcmp(sVal, "define") == 0) {
        //add defined value to symbol table
        push(newAtom(oDEFINE, NULL, NULL));
        if (iHEAD == NULL) {
            //set the head
            iHEAD = newIdentifier(NULL, NULL, NULL);
        }
        else {
            setCurrentIdentifier();
            iCURR->next = newIdentifier(NULL, NULL, NULL);
        }
    }
    else if (strcmp(sVal, "quote") == 0) {
        //Add next value to list
        push(newAtom(oQUOTE, NULL, NULL));
    }
    else if (strcmp(sVal, "list") == 0) {
        //return a list of the atoms
        push(newAtom(oLIST, NULL, newIdentifier("(", NULL, newIdentifier(")", NULL, NULL))));
    }
    else if (pos != -1 && stack[pos]->type == oQUOTE){
        //add atom to list, do not evaluate
        addToList(sVal);
    }
    else {
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
                    if (iCURR-> name != NULL && strcmp(sVal, iCURR->name) == 0) {
                        //identifier found, perform operation
                        performOperation(iCURR->data);
                        break;
                    }
                    //not the head value, iterate through the rest
                    else {
                        while (iCURR != NULL) {
                            if (iCURR-> name != NULL && strcmp(sVal, iCURR->name) == 0) {
                                //identifier found
                                performOperation(iCURR->data);
                                break;
                            }
                            iCURR = iCURR->next;
                        }
                    }
                }
                if (iCURR == NULL) {
                    //if this is reached identifier was not found
                    fatalError ("Unknown identifier");
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
        if(pos >= 0 && result->iVal != NULL) {
            performOperation(result->iVal);
        }
    }
}

void push(atom *at) {
    //add a value to the end of the stack
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
    else if (at->type == oLIST) {
        //add to the end of the list
        addToList((char*) at->iVal);
    }
    else if (at->iVal == NULL) {
        //switch (at->type) {
            //TODO: might be able to get rid of this
            //default:
                //add, sub, mult, divide
                //set first value to operate on
                at->iVal = value;
                //break;
        //}
    } else {
        switch (at->type) {
            case oADD:
                //add to value
                printf("%d + %d\n", stack[pos]->iVal, value);
                at->iVal = at->iVal + value;
                break;
            case oSUB:
                //sub from value
                printf("%d - %d\n", stack[pos]->iVal, value);
                at->iVal = at->iVal - value;
                break;
            case oMULT:
                //multiply value
                at->iVal = at->iVal * value;
                break;
            case oDIV:
                //divide value
                at->iVal = at->iVal / value;
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

void removeIdentifier(char *sVal) {
    //check defined identifiers
    if (iHEAD != NULL) {
        iCURR = iHEAD;
        if (iCURR-> name != NULL && strcmp(sVal, iCURR->name) == 0) {
            //identifier found, set as head to next which should exist
            iHEAD = iCURR->next;
            return;
        }
        //not the head value, check the rest
        else {
            while (iCURR != NULL) {
                if (iCURR->next != NULL && iCURR->next->name != NULL && strcmp(sVal, iCURR->next->name) == 0) {
                    //identifier found, remove
                    if (iCURR->next->next != NULL) {
                        //set next to skip value
                        iCURR->next = iCURR->next->next;
                    }
                    else {
                        //now the end of the list
                        iCURR->next = NULL;
                    }
                    return;
                }
                iCURR = iCURR->next;
            }
        }
    }
}

void addToList(char *sVal) {
    //go to end of list
    identifier *endOfList = stack[pos-1]->listHead;
    while (endOfList->next->name != ")") {
        endOfList = endOfList->next;
    }
    //add new value as next
    endOfList->next = newIdentifier(sVal, NULL, newIdentifier(")", NULL, NULL));
}

atom* evalList(exprs *l, int n) {
    if (l == NULL) return NULL;
    evalExpr(l->e, n);
    evalList(l->n, n);
    //return end result from last value in stack
    return result;
}

/* end of eval.c */
