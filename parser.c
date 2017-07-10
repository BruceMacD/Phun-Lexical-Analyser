/*
 * Simple Expression Interpreter
 * AST creation and evaluation
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

//track open vs close brackets to validate syntax
int brackets = 0;

/*
 * build ast from nodes and leaves
 */
node *parseExpr (token t) {
    node *n1;
    switch (t.type) {

        case tQUOTE:
            //parse to find expr node first from a token
            t = scan();
            node * nExpr;
            //add leaf depending on next expr
            switch (t.type) {
                case tIDENT:
                    nExpr = newNode(astEXPR, newLeaf(astIDENT, NULL, strdup(t.sVal)), NULL);
                    break;

                case tINT:
                    nExpr = newNode(astEXPR, newLeaf(astINT, t.iVal, NULL), NULL);
                    break;

                case tSTRING:
                    nExpr = newNode(astEXPR, newLeaf(astSTRING, NULL, strdup(t.sVal)), NULL);
                    break;

                default:
                    //next token not expected
                    fatalError ("Syntax Error");
            }
            //create new node, add quote leaf and expr
            //Expr -> ' Expr
            node * nQuote = (newNode(astEXPR, newLeaf(astQUOTE, NULL, "Quote"), nExpr));
            //continue building the tree from the next token
            n1 = parseExpr(scan());
            //return node Exprs -> Expr Exprs
            return newNode(astEXPRS, nQuote, n1);

        case tIDENT:
            //continue building the tree
            n1 = parseExpr(scan());
            //create identifier leaf
            //Expr -> id
            node * nIdent = newNode(astEXPR, newLeaf(astIDENT, NULL, strdup(t.sVal)), NULL);
            //add identifier leaf, add child node to lower part of tree
            //Exprs -> Expr Exprs
            return newNode(astEXPRS, nIdent, n1);

        case tINT:
            n1 = parseExpr(scan());
            //create new node, add integer leaf, add child node
            //Expr -> int
            node * nInt = newNode(astEXPR, newLeaf(astINT, t.iVal, NULL), NULL);
            //Exprs -> Expr Exprs
            return newNode(astEXPRS, nInt, n1);

        case tSTRING:
            n1 = parseExpr(scan());
            //create new node, add string leaf, add child node
            node * nString = newNode(astEXPR, newLeaf(astSTRING, NULL, strdup(t.sVal)), NULL);
            //Exprs -> Expr Exprs
            return newNode(astEXPRS, nString, n1);

        case tBEGIN:
            //track bracket count to validate syntax
            brackets = brackets + 1;
            //get the rest of the tree
            n1 = parseExpr(scan());
            // List -> ( Exprs )
            node * nList = newNode(astLIST, newLeaf(astBEGIN, NULL, "("), n1);
            //Expr -> List
            node * beginExprNode = newNode(astEXPR, newNode(astEXPR, nList, NULL), NULL);
            //Exprs -> Expr Exprs
            return newNode(astEXPRS, beginExprNode, newNode(astEXPRS, NULL, NULL));

        case tEND:
            //check for close bracket before open bracket
            if (brackets > 0)
            {
                brackets = brackets - 1;
            } else {
                fatalError ("Syntax Error - Closing bracket without matching opening");
            }
            n1 = parseExpr(scan());
            //create new node, add string leaf, add child node
            node * nEND = newNode(astEXPR, newLeaf(astEND, NULL, ")"), NULL);
            //Exprs -> Expr Exprs
            return newNode(astEXPRS, nEND, n1);

        case tEOF:
            //end of file, stop recursion and add terminating node with terminating leaves
            return (newNode(astEXPRS, newLeaf(astEOF, NULL, NULL), NULL));

        default:
            //something went wrong
            fatalError ("Syntax Error");
    }
}

node * parse () {
    //create the ast from the bottom up
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