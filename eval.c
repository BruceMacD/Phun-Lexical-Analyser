/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

operation *newOperation (operationType type, operation *next, int result) {
    operation *o = malloc(sizeof (operation));
    o->type = type;
    o->nextOperation = next;
    o->result = result;
    return (o);
}

//tracks amount of indentation between nodes
int   indent = 0;
//start with no set operation
operation *oHead = NULL;
operation *o = NULL;
int opResult;

/*
 * parse an ast and print it
 * TODO: May need to change return type, see usage in main
 */
int evaluate (node *ast) {

    switch (ast->type) {

        case astEXPRS:
            //node found, parse leaves if not empty
            if(ast->operand1 != NULL) {
                evaluate(ast->operand1);
            }
            if(ast->operand2 != NULL) {
                evaluate(ast->operand2);
            }
            break;

        case astEXPR:
            //node found, parse leaves if not empty
            if(ast->operand1 != NULL) {
                evaluate(ast->operand1);
            }
            if(ast->operand2 != NULL) {
                evaluate(ast->operand2);
            }
            break;

        case astLIST:
            //node found, parse leaves if not empty
            //list has 3 leaves ( Exprs )
            if(ast->operand1 != NULL) {
                evaluate(ast->operand1);
            }
            if(ast->operand2 != NULL) {
                // new operation
                evaluate(ast->operand2);
            }
            break;

        case astBEGIN:
            //for evaluation
            indent = indent + 1;
            break;

        case astEND:
            //done performing current operation
            //transfer the result to the parent operation
            indent = indent - 1;
            opResult = o->result;
            removeLastOperation();
            //TODO: needs cases
            //check if last operation
            if (indent != 0) {
                switch (o->type) {
                    case oADD:
                        o->result = o->result + opResult;
                        break;
                    case oSUB:
                        o->result = o->result - opResult;
                        break;
                    case oMULT:
                        o->result = o->result * opResult;
                        break;
                    case oDIV:
                        o->result = o->result / opResult;
                        break;
                    default:
                        fatalError("Invalid identifier");
                        break;
                }
            }
            break;
        case astIDENT:
            //TODO: check more ops and consider moving to separate function
            switch (*ast->sVal) {
                case '+':
                    //node *operand1 = evaluate(n->operand1);
                    //set operation to addition
                    if ( oHead != NULL) {
                        //set operation
                        findCurrentOperation();
                        o->nextOperation = newOperation(oADD, NULL, NULL);
                    } else {
                        //operation has not been set yet
                        oHead = newOperation(oADD, NULL, NULL);
                    }
                    break;
                case '-':
                    //set operation to subtraction
                    if ( oHead != NULL) {
                        //set operation
                        findCurrentOperation();
                        o->nextOperation = newOperation(oSUB, NULL, NULL);
                    } else {
                        //operation has not been set yet
                        oHead = newOperation(oSUB, NULL, NULL);
                    }
                    break;
                case '*':
                    //set operation to multiplication
                    if ( oHead != NULL) {
                        //set operation
                        findCurrentOperation();
                        o->nextOperation = newOperation(oMULT, NULL, NULL);
                    } else {
                        //operation has not been set yet
                        oHead = newOperation(oMULT, NULL, NULL);
                    }
                    break;
                case '/':
                    //set operation to division
                    if ( oHead != NULL) {
                        //set operation
                        findCurrentOperation();
                        o->nextOperation = newOperation(oDIV, NULL, NULL);
                    } else {
                        //operation has not been set yet
                        oHead = newOperation(oDIV, NULL, NULL);
                    }
                    break;
                default:
                    fatalError("Invalid identifier");
                    break;
            }
            //printLeaf(ast);
            break;
        case astSTRING:
            //string leaf
            //TODO: Check in symbol table
            printIndent();
            printLeaf(ast);
            break;
        case astQUOTE:
            //quote leaf
            //TODO: Strip quote and print
            printIndent();
            printLeaf(ast);
            break;
        case astINT:
            //integer leaf
            findCurrentOperation();
            //perform operation
            //TODO: maybe move to separate function
            //TODO: remove printf, it is for debug
            switch (o->type) {
                case oADD:
                    //check if not first operation
                    if (o->result != NULL) {
                        //add to return value
                        printf("Current: %d + %d\n", o->result, ast->iVal);
                        o->result = o->result + ast->iVal;
                        printf("Result: %d\n", o->result);
                    } else {
                        //set value to operate on
                        printf("Setting base value: %d\n", ast->iVal);
                        o->result = ast->iVal;
                    }
                    break;
                case oSUB:
                    //check if not first operation
                    if (o->result != NULL) {
                        //subtract from return value
                        printf("Current: %d - %d\n", o->result, ast->iVal);
                        o->result = o->result - ast->iVal;
                        printf("Result: %d\n", o->result);
                    } else {
                        //set value to operate on
                        o->result = ast->iVal;
                    }
                    break;
                case oMULT:
                    //check if not first operation
                    if (o->result != NULL) {
                        //multiply return value
                        printf("Current: %d - %d\n", o->result, ast->iVal);
                        o->result = o->result * ast->iVal;
                        printf("Result: %d\n", o->result);
                    } else {
                        //set value to operate on
                        o->result = ast->iVal;
                    }
                    break;
                case oDIV:
                    //check if not first operation
                    if (o->result != NULL) {
                        //divide return value
                        printf("Current: %d - %d\n", o->result, ast->iVal);
                        o->result = o->result / ast->iVal;
                        printf("Result: %d\n", o->result);
                    } else {
                        //set value to operate on
                        o->result = ast->iVal;
                    }
                    break;
                default:
                    fatalError("Invalid identifier");
                    break;
            }
            break;
        case astEOF:
            //end of file, do nothing
            break;
        default:
            fatalError("Unknown AST node");
            break;
    }
    if (o != NULL) {
        return o->result;
    }
#ifdef debug
    printf("Node evaluated to: %d\n", result);
#endif
}

void findCurrentOperation() {
    //iterate to end of list to find current operation
    //TODO: This could be more efficient if we stored the list in reverse. Could just return head
    o = oHead;
    while (o->nextOperation != NULL) {
        o = o->nextOperation;
    }
}

void removeLastOperation() {
    //iterate to second last in list
    o = oHead;
    if (o->nextOperation != NULL) {
        while (o->nextOperation->nextOperation != NULL) {
            o = o->nextOperation;
        }
        //remove from list
        o->nextOperation = NULL;
    }
}

void printIndent() {
    //set the amount of indentation
    int i;
    for( i = 0; i < indent; i = i + 1 ){
        printf("\t");
    }
}

//print leaf in desired format
void printLeaf (node *n) {
    switch (n->type) {
        case astBEGIN: printf(n->sVal); break;
        case astEND: printf(n->sVal); break;
        case astIDENT: printf("Identifier: [%s]", n->sVal); break;
        case astSTRING: printf("String: [%s]", n->sVal); break;
        case astQUOTE: printf("Quote"); break;
        case astINT: printf("Integer: [%d]", n->iVal); break;
        case astEOF: printf("End of File"); break;
        default: break;
    }
    printf("\n");
}

/* end of eval.c */
