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
//arbitrary maximum size
#define MAXSIZE 1024

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
 * Symbol table for storing identifier values
 */
typedef struct symbol
{
    char* identifier;
    //TODO: Types other than ints?
    int data;
    // pointer to next value in the table
    struct identifier* next;
} symbol;

/*
 * States for operations
 */
//TODO: Expand operationTypes
typedef enum { oADD, oSUB , oMULT, oDIV, oCAR, oCDR, oLIST, oCONS, oDEFINE } identifierType;

/*
 * For tracking current operation
 */
typedef struct operation
{
    identifierType type;
    // pointer to next operation
    struct operation* nextOperation;
    // store running result
    int result;
} operation;

/*
 * Function Declarations
 */
void fatalError (char *msg);
char nextChar ();
void returnChar (char c);

void printToken (token t);
token scan ();

exprs *parse();
exprs *parseFileList (token t);
exprs *parseExprList (token t);
expr  *parseExpr (token t);
int evalList(exprs *l, int n);
void symbolTable(char* sVal);
void pop();
void push(operation *op);
void performOperation(int value);

/* end of phun.h */