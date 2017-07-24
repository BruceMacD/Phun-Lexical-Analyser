/* 
 * Phun Interpreter
 * Tami Meredith, June 2017
 */
 
/*
 * Constants
 */
#define SUCCESS  0
#define FAILURE -1
#define DEBUG    1  /* Set to 0 to turn off debugging messages */

/*
 * Types for lexical analysis
 */
typedef enum { sSTART, sINT, sIDENT, sSTRING, sCOMMENT } state;
typedef enum { tBEGIN, tEND, tQUOTE, tINT, tIDENT, tSTRING, tEOF } tokentype;

typedef struct {
    tokentype type;
    int       iVal;
    char     *sVal;    
} token;

/*
 * Types for parsing and AST construction
 */
typedef enum { eString, eIdent, eInt, eExprList } exprtype;

typedef struct expression {
    exprtype type;
    char    *sVal;
    int      iVal;
    struct exprList *eVal;
} expr;

typedef struct exprList {
    expr *e;
    struct exprList *n;
} exprs;

/*
 * Types for super simple symbol tables
 * - An element in list of symbols 
 */
typedef struct symbolS {
  char *name;
  expr *data;
  struct symbolS *next;   /* ptr to next node */
} symbol;

/* SymbolTable ADT - a linked list */
typedef struct symtabS {
    int     length;   /* length */
    symbol *first;    /* ptr to first symbol */
} symtab;

typedef struct symbolF {
    char *name;
    expr *operation;
    symtab fst;
    struct symbolF *next;   /* ptr to next node */
} function;

/* FunctionTable ADT - a linked list */
typedef struct funtabF {
    int     length;   /* length */
    function *first;    /* ptr to first symbol */
} funtab;

/* ---------------------------- PROTOTYPES ---------------------------- */

void fatalError (char *msg);
char nextChar ();
void returnChar (char c);

symbol *lookup(char *name);
symbol *bind(char *name, expr *val);

function *lookupFunction(char *name);
function *bindFunction(char *name, expr *val);

void printToken (token t);
token scan ();

expr *newStringExpr(char *s);
expr *newIdentExpr(char *s);
expr *newListExpr(exprs *l);
expr *newIntExpr(int i);
exprs *newExprList(expr *e, exprs *n);

exprs *parse();
exprs *parseFileList (token t);
exprs *parseExprList (token t);
expr  *parseExpr (token t);

void listPrint(exprs *l);
void exprPrint(expr *e);

expr *eval(expr *e);

/* end of phun.h */