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
 * This is the rule for S:
 * - there is a case for every token in its predictor set
 * - the default action is for syntax errors
 */
node *parseExpr (token t) {
    node *n1, *n2;
    //TODO: Figure out how ( Exprs ) works here
    //TODO: How to do list
    switch (t.type) {
        case tQUOTE:
            n1 = parseExpr(scan());
            return (newNode(astQUOTE, n1, NULL));

        case tIDENT:
            return (newLeaf(astIDENT, t.iVal, strdup(t.sVal)));

        case tINT:
            return (newLeaf(astINT, t.iVal, strdup(t.sVal)));

        case tSTRING:
            return (newLeaf(astSTRING, t.iVal, strdup(t.sVal)));

        case tBEGIN:
            n1 = parseExpr(scan());
            return (newNode(astBEGIN, n1, NULL));

        default:
            /* Oh noes, something went awry! */
            fatalError ("Syntax Error");
    }
}

node* parse () {
    return (parseExpr(scan()));
}

/* end of parser.c */