/* 
 * Phun Interpreter
 * Rebecca Ansems, Brandon Bremner, Bruce MacDonald, June 2017
 *
 * Built on provided sample code from: http://www.tamimeredith.ca/downloads/csci3136/sexi.tar
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
 * Types for AST generation
 */
typedef enum { astBEGIN, astEND, astIDENT, astSTRING, astQUOTE, astINT, astEOF, astEXPRS, astEXPR, astLIST } asttype;

typedef struct astS {
    asttype     type;
    struct astS *operand1;
    struct astS *operand2;
    int         iVal;
    char       *sVal;
} node;

/*
 * Symbol table for storing identifier values
 */
typedef struct symbolTable
{
    char* name;
    //TODO: Types other than ints?
    int data;
    // pointer to next value in the table
    struct symbol* next;
} symbolTable;

/*
 * States for operations
 */
//TODO: Expand operationTypes
typedef enum { oADD, oSUB , oMULT, oDIV } operationType;

/*
 * For tracking current operation
 */
typedef struct operation
{
    operationType type;
    // pointer to previous operation
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

node *parse();
node *parseExpr(token t);

int evaluate(node *n);
void findCurrentOperation();
void removeLastOperation();
void setOperation(node *ast);
void performOperation(node *ast);
void printIndent();
void printLeaf (node *n);
void identifyOperation (node *n);
/* end of phun.h */
