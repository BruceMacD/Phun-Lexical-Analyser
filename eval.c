/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

//tracks amount of indentation between nodes
int   indent = 0;

/*
 * parse an ast and print it
 */
void evaluate (node *ast) {

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
            printIndent();
            printLeaf(ast);
            break;
        case astSTRING:
            //string leaf
            printIndent();
            printLeaf(ast);
            break;
        case astQUOTE:
            //quote leaf
            printIndent();
            printLeaf(ast);
            break;
        case astINT:
            //integer leaf
            printIndent();
            printLeaf(ast);
            break;
        case astEOF:
            //end of file, do nothing
            break;
        default:
            fatalError("Unknown AST node");
            break;
    }
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

/* end of eval.c */
