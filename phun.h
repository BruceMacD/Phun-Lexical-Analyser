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
    struct astS *operand3;
    int         iVal;
    char       *sVal;
} node;

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

void evaluate(node *n);
void printIndent();
void printLeaf (node *n);
/* end of phun.h */
