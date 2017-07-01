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

//TODO: move to eval?
void parseExpr (token t, int indent) {

    switch (t.type) {
        case tQUOTE:
            printIndent(indent);
            printf("'\n");
            parseExpr(scan(), indent);
            break;
        case tIDENT:
            printIndent(indent);
            printf("Identifier: [%s]\n", t.sVal);
            parseExpr(scan(), indent);
            break;
        case tINT:
            printIndent(indent);
            printf("Integer: [%d]\n", t.iVal);
            parseExpr(scan(), indent);
            break;
        case tSTRING:
            printIndent(indent);
            printf("String: [%s]\n", t.sVal);
            parseExpr(scan(), indent);
            break;
        case tBEGIN:
            printIndent(indent);
            printf("(\n");
            parseExpr(scan(), indent + 1);
            break;
        case tEND:
            indent = indent - 1;
            printIndent(indent);
            printf(")\n");
            parseExpr(scan(), indent);
            break;
        case tEOF:
            //do nothing, end of file
            break;
        default:
            /* Oh noes, something went awry! */
            fatalError ("Syntax Error");
    }
}

void printIndent(int indent) {
    //set the amount of indentation
    int i;
    for( i = 0; i < indent; i = i + 1 ){
        printf("\t");
    }
}

void parse () {
    return (parseExpr(scan(), 0));
}

/* end of parser.c */