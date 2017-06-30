/*
 * Simple Expression Interpreter
 * -- Stage 3: AST creation and evaluation
 * Tami Meredith, June 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "phun.h"

/*
 * Create a new AST Node (non-leaf)
 */
node *newNode (asttype type, node* lhs, node* rhs) {
    node *n = malloc(sizeof (node));
    n->type = type;
    n->operand1 = lhs;
    n->operand2 = rhs;
    return (n);
}

/*
 * Create a new AST Leaf
 */
node *newLeaf (asttype type, int iValue, char* sValue) {
    node *n = malloc(sizeof (node));
    n->type = type;
    n->iVal = iValue;
    n->sVal = sValue;
    return (n);
}

/*
 * build ast from nodes and leaves
 */
/*
node *parseExpr (token t) {
    node *n1, *n2;
    //TODO: How to do list
    //TODO: When to use left vs right leaf
    switch (t.type) {
        case tQUOTE:
            n1 = parseExpr(scan());
            return (newNode(astQUOTE, NULL, n1));

        case tIDENT:
            n1 = parseExpr(scan());
            return (newNode(astIDENT, n1, NULL));

        case tINT:
            n1 = parseExpr(scan());
            return (newNode(astINT, n1, NULL));

        case tSTRING:
            n1 = parseExpr(scan());
            return (newNode(astSTRING, n1, NULL));

        case tBEGIN:
            n1 = parseExpr(scan());
            return (newNode(astBEGIN, n1, NULL));

        case tEND:
            //end if statement, return and recurse
            return (newLeaf(astEND, t.iVal, strdup(t.sVal)));

        //TODO: this may be needed to catch errors
        //case tEOF:

        default:
            //something went wrong
            fatalError ("Syntax Error");
    }
}*/

void parseExpr (token t) {
    switch (t.type) {
        case tQUOTE:
            printf("'\n");
            parseExpr(scan());
            break;
        case tIDENT:
            printf("Identifier: [%s]\n", t.sVal);
            parseExpr(scan());
            break;
        case tINT:
            printf("Integer: [%d]\n", t.iVal);
            break;
        case tSTRING:
            printf("String: [%s]\n", t.sVal);
            break;
        case tBEGIN:
            printf("(\n");
            //add an indent
            printf("\t");
            parseExpr(scan());
            break;
        case tEND:
            printf(")\n");
            break;
        default:
            /* Oh noes, something went awry! */
            fatalError ("Syntax Error");
    }
}

void parse () {
    return (parseExpr(scan()));
}

/* end of parser.c */