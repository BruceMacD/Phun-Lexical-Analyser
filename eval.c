/*
 * AST Evaluator
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "phun.h"

/*
 * parse an ast and print it
 */
void evaluate (node *ast) {
    int result;
    //char * sResult;
    switch (ast->type) {
        case astBEGIN:
            //example:
            //result = evaluate(ast->operand1) + evaluate(ast->operand2);
            //sresult = ast->sVal;
            break;
        case astEND:
            //sresult = ast->sVal;
            break;
        case astIDENT:
            //sresult = ast->sVal;
            break;
        case astSTRING:
            //sresult = ast->sVal;
            break;
        case astQUOTE:
            //sresult = ast->sVal;
            break;
        case astINT:
            result = ast->iVal;
            break;
        default:
            fatalError("Unknown AST node");
            result = 0; /* dead code, assignment shouldn't happen */
            break;
    }
#ifdef debug
    printf("Node evaluated to: %d\n", result);
#endif
}

/* end of eval.c */
