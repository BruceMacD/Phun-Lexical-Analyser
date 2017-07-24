/* 
 * Phun Interpreter
 * Evaluator Code
 * Tami Meredith, July 2017
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "phun.h"

#define SUM  0
#define DIFF 1
#define PROD 2
#define DIV  3

/*
 * Evaluate the arguments for specific keywords and produce an expression
 */
expr *doQuote(exprs *ls) {
    expr *val;

    if (ls == NULL)
        fatalError("Missing value for quote");
    val = ls->e;
    if (ls->n != NULL)
        fatalError("Extra expressions in a quote");
    /* Quoted, don't evaluate */
    return (val);
}

expr *doCar(exprs *ls) {
    expr *head;
    if (ls == NULL)
        fatalError("Missing argument for car");
    head = eval(ls->e);
    if (head->type != eExprList) 
        fatalError("car not used on a list");      
    return (head->eVal->e);
}

expr *doCdr(exprs *ls) {
    expr *tail;
    if (ls == NULL)
        fatalError("Missing argument for cdr");
    tail = eval(ls->e);
    if (tail->type != eExprList) 
        fatalError("cdr not used on a list");      
    return (newListExpr(tail->eVal->n));
}

expr *doCons(exprs *ls) {
    expr *head, *tail;
    if (ls == NULL)
        fatalError("Missing values for cons");
    head = eval(ls->e);
    ls = ls->n;
    if (ls == NULL)
        fatalError("Missing second value for cons");
    tail = eval(ls->e);
    if (tail->type != eExprList) 
        fatalError("Second value for cons is not a list");
    
    return (newListExpr (newExprList (head, tail->eVal))); 
}

expr *doList(exprs *ls) {
    expr *head, *tail;
    if (ls == NULL)
        return (newListExpr (ls));
    head = eval (ls->e);
    tail = doList(ls->n);
    if (tail == NULL)
        return (newListExpr (newExprList (head, NULL)));
    else
        return (newListExpr (newExprList (head, tail->eVal)));
}

expr *doDefine(exprs *ls) {
    expr *name;
    expr *val;
    symbol *s;
    if (ls == NULL)
        fatalError("Definition missing name and value");
    name = ls->e;
    if (name->type != eIdent)
        fatalError("Missing identifier in a definition");
    ls = ls->n;
    if (ls == NULL)
        fatalError("Definition missing a value");
    val = ls->e;
    if (ls->n != NULL)
        fatalError("Extra expressions in a definition");
    val = eval(val);
    s = lookup(name->sVal);
    if (s != NULL) {
        s->data = val;
    } else {
        bind(name->sVal,val);
    }
    return(NULL); /* don't want main to print anything, so return NULL */
}

expr *doParams(exprs *ls) {
    expr *name;
    expr *val;
    symbol *s;
    if (ls == NULL)
        fatalError("Definition missing name and value");
    name = ls->e;
    if (name->type != eIdent)
        fatalError("Missing identifier in a definition");
    ls = ls->n;
    if (ls == NULL)
        fatalError("Definition missing a value");
    val = ls->e;
    if (ls->n != NULL)
        fatalError("Extra expressions in a definition");
    val = eval(val);
    s = lookup(name->sVal);
    if (s != NULL) {
        s->data = val;
    } else {
        bind(name->sVal,val);
    }
    return(NULL); /* don't want main to print anything, so return NULL */
}

expr *doDefineFunction(exprs *ls) {
    expr *name;
    expr *operation;
    function *f;
    if (ls == NULL)
        fatalError("Definition missing name and value");
    name = ls->e;
    if (name->type != eIdent)
        fatalError("Missing identifier in a definition");
    ls = ls->n;
    if (ls == NULL)
        fatalError("Definition missing a value");
    operation = ls->e;
    if (ls->n != NULL)
        fatalError("Extra expressions in a definition");
    //val = eval(val);
    f = lookupFunction(name->sVal);
    if (f != NULL) {
        f->operation = operation;
    } else {
        bindFunction(name->sVal, operation);
    }
    return(NULL); /* don't want main to print anything, so return NULL */
}

/*
 * Evaluate the arguments for binary+ operators and then apply the operation
 * - repeat as necessary for 3+ arguments in the list
 */
expr *doBinaryOp(int op, exprs *ls) {
    expr *val1, *val2;
    int   i, j, k;
    if (ls == NULL)
        fatalError("Missing values for binary operator");
    val1 = eval(ls->e);
    if (val1->type != eInt) 
        fatalError("First value for binary operation is not an int");      
    i = val1->iVal;
getAnother:
    ls = ls->n;
    if (ls == NULL)
        fatalError("Missing second+ value for binary operation");
    val2 = eval(ls->e);
    if (val2->type != eInt) 
        fatalError("Second+ value for binary operation is not an int");
    j = val2->iVal;
    switch (op) {
        case SUM:  k = i + j; break;
        case DIFF: k = i - j; break;
        case PROD: k = i * j; break;
        case DIV:  k = i / j; break;
        default:
            fatalError("Unknown error: Applying invalid operation");
    }
    i = k;
    if (ls->n != NULL) goto getAnother;
    return (newIntExpr(k));   
}

/*
 * Evaluate an Expression
 */ 
expr *eval(expr *e) {
    expr   *op;
    exprs  *list;
    symbol *s;
    function *f;
    
    switch (e->type) {
        case eString:
            /* Not needed in assignment 6 */
            /* Fall through */
        case eInt:
            return (e);
            break;
        case eIdent:
            s = lookup(e->sVal);
            if (s == NULL)
                fatalError("Unbound symbol");
            return (s->data);
            break;
        case eExprList:
            list = e->eVal;
            op = list->e;
            list = list->n;
            if (op->type != eIdent) {
                fatalError("Invalid operator in function application");
            }
            if (!strcasecmp (op->sVal,"define")) {
                //TODO: This is very fragile, could result in a seg fault easily
                if (list->n->e->eVal != NULL && !strcasecmp (list->n->e->eVal->e->sVal,"lambda")) {
                    // This means a function is being defined, add it to the function table
                    //read the entire enclosed function
                    return(doDefineFunction(list));
                }
                return (doDefine(list));
            } else if (!strcasecmp (op->sVal,"lambda")) {
                return (doParams(list));
            } else if (!strcasecmp (op->sVal,"car")) {
                return (doCar(list));
            } else if (!strcasecmp (op->sVal,"cdr")) {
                return (doCdr(list));
            } else if (!strcasecmp (op->sVal,"cons")) {
                return (doCons(list));
            } else if (!strcasecmp (op->sVal, "quote")) {
                return (doQuote(list));
            } else if (!strcasecmp (op->sVal,"list")) {
                return (doList(list));
            } else if (!strcmp (op->sVal,"+")) {
                return (doBinaryOp(SUM,list));
            } else if (!strcmp (op->sVal,"-")) {
                return (doBinaryOp(DIFF,list));
            } else if (!strcmp (op->sVal,"*")) {
                return (doBinaryOp(PROD,list));
            } else if (!strcmp (op->sVal,"/")) {
                return (doBinaryOp(DIV,list));
            } else {
                //check defined functions
                f = lookupFunction(e->eVal->e->sVal);
                if (f == NULL)
                    fatalError("Unbound operator in function application");
                //put the values in the symbol table
                symbol *param = f->fst.first;
                //iterate through each value setting the data and binding it
                for (int i = 0; i < f->fst.length; i++) {
                    param->data = list->e;
                    bind(param->name, param->data);
                    param = param->next;
                    list = list->n;
                }
                return (eval(f->operation));
            }                
            break;
        default:
            break;
    }
}

/*
 * Print expressions in the format needed by the interpreter
 */
void exprPrint(expr *e) {
    switch (e->type) {
        case eString:
            /* fall through */
        case eIdent:
            printf("%s", e->sVal);
            break;
        case eInt:
            printf("%d", e->iVal);
            break;
        case eExprList:
            printf("(");
            listPrint(e->eVal);
            printf(")");
            break;
        default:
            break;
    }
}
 
void listPrint(exprs *l) {
    if (l == NULL) return;
    exprPrint(l->e);
    if (l->n != NULL) {
      printf(" ");
      listPrint(l->n);
    }
}

/* end of eval.c */