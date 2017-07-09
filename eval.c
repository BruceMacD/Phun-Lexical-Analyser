/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

operation *newOperation (operationType type, operation *prev, operation *next) {
    operation *o = malloc(sizeof (operation));
    o->type = type;
    o->previousOperation = prev;
    o->nestedOperation = next;
    return (o);
}

//tracks amount of indentation between nodes
int   indent = 0;
//start with no set operation
operation *o = NULL;
int result = 0;

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
            if(ast->operand3 != NULL) {
                evaluate(ast->operand3);
            }
            break;

        case astBEGIN:
            //open bracket
            printIndent();
            //increase indent for following
            indent = indent + 1;
            printLeaf(ast);
            break;
        case astEND:
            //close bracket, decrease indent for following
            indent = indent - 1;
            printIndent();
            printLeaf(ast);
            break;
        case astIDENT:
            //identifier leaf
            //check the operation
            //find the arguments
            //printIndent();
            //set operation
            //TODO: check more ops and consider moving to separate function
            switch (*ast->sVal) {
                case '+':
                    //node *operand1 = evaluate(n->operand1);
                    //set operation to addition
                    if ( o != NULL) {
                        //TODO: might be overwriting o incorrectly here
                        operation *newO = newOperation(oADD, o, NULL);
                        o->nestedOperation = newO;
                        o = newO;
                    } else {
                        //operation has not been set yet
                        o = newOperation(oADD, NULL, NULL);
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
            //printIndent();
            //printLeaf(ast);
            //perform operation
            //TODO: maybe move to separate function
            switch (o->type) {
                case oADD:
                    //add to return value
                    result = result + ast->iVal;
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
    return result;
#ifdef debug
    printf("Node evaluated to: %d\n", result);
#endif
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
/*
void identifyOperation (node *n) {
    //TODO: Finish different cases for different identifiers
    switch (*n->sVal) {
        case '+':
            //node *operand1 = evaluate(n->operand1);
            result = result +
            break;
        case '-':
            break;
        case '*':
            break;
        case '/':
            break;
        case 'car':
            break;
        default:
            fatalError("Invalid identifier");
            break;
    }
}*/

/* end of eval.c */
