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

node *newListNode (asttype type, node* lhs, node* rhs) {
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

//track open vs close brackets to validate syntax
int brackets = 0;

/*
 * build ast from nodes and leaves
 */
node *parseExpr (token t) {
    node *n1;
    switch (t.type) {

        case tQUOTE:
            //parse to find child first
            n1 = parseExpr(scan());
            //create new node, add quote leaf, add child node
            return (newNode(astEXPRS, newLeaf(astQUOTE, NULL, "Quote"), n1));

        case tIDENT:
            n1 = parseExpr(scan());
            //create new node, add identifier leaf, add child node
            return (newNode(astEXPRS, newLeaf(astIDENT, NULL, strdup(t.sVal)), n1));

        case tINT:
            n1 = parseExpr(scan());
            //create new node, add integer leaf, add child node
            return (newNode(astEXPRS, newLeaf(astINT, t.iVal, NULL), n1));

        case tSTRING:
            n1 = parseExpr(scan());
            //create new node, add string leaf, add child node
            return (newNode(astSTRING, newLeaf(astSTRING, NULL, strdup(t.sVal)), n1));

        case tBEGIN:
            brackets = brackets + 1;
            n1 = parseExpr(scan());
            //create new node, add open bracket leaf, add child node
            return (newNode(astEXPRS, newLeaf(astBEGIN, NULL, "("), n1));

        case tEND:
            //check for close bracket before open bracket
            if (brackets > 0)
            {
                brackets = brackets - 1;
            } else {
                fatalError ("Syntax Error - Closing bracket without matching opening");
            }
            n1 = parseExpr(scan());
            //create new node, add close bracket leaf, add child node
            return (newNode(astEXPRS, newLeaf(astEND, NULL, ")"), n1));

        case tEOF:
            //end of file, stop recursion and add terminating node with terminating leaves
            return (newNode(astEXPRS, newLeaf(astEOF, NULL, NULL), NULL));

        default:
            //something went wrong
            fatalError ("Syntax Error");
    }
}

node * parse () {
    //create the ast
    node * root = (parseExpr(scan()));
    //check for valid brackets before returning
    if (brackets == 0) {
        return root;
    } else {
        //not valid bracket syntax
        fatalError ("Syntax Error");
    }
}

/* end of parser.c */