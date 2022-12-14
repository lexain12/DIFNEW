#pragma once
#include <stdlib.h>

extern const char   GraphFile[20];
extern const size_t MAXDATASIZE;
extern const size_t MAXCMDSIZE;
extern const char SPEAKFILE[40];
extern const char* ShortCMD;
extern const double EPS;
extern const double VarPoison;

extern const size_t VarTableSize;
extern const char* LatexFileName;

extern const size_t numOfFillers;
extern const char* fillers[];

enum Errors
{
    noErrors     = 0,
    treePtrError = 1 << 0,
};

enum OP
{
    UnknownOp = 0,
    OP_ADD    = 1,
    OP_SUB    = 2,
    OP_MUL    = 4,
    OP_DIV    = 5,
    OP_POW    = 6,
    OP_COS    = 7,
    OP_SIN    = 8,
    OP_LOG    = 9,
    OP_LN     = 10,
    OP_LBR    = 11,
    OP_RBR    = 12,
};

enum Type
{
    Unknown = 0,
    OP_t    = 1,
    Var_t   = 2,
    Num_t   = 3,
};

struct Node 
{
    Type   type;
    OP     opValue;
    double numValue;
    char*  varName;
    Node*  left;
    Node*  right;
};

struct Tree
{
    Node*  root;
    size_t size;
    int    status;
};

struct Var 
{
    char* varName;
    double varValue; 
};

Node* nodeCtor ();
void nodeDtor (Node* node);
Node* createNode (Type type, OP opValue, double num, char* varValue, Node* left, Node* right);

int treeCtor (Tree* tree);

void  treePrint        (const Node* node, int isPrint,           FILE* const fileToPrint);
void  makeGraph        (Tree* tree);
void  treeGraph        (const Node* node, FILE* GraphFilePtr);

void  inputCleaning();
int   compareOperation (const Node* node, const Node* childNode);
void  printOperation   (const Node* node, const char* operation, FILE* const fileToPrint);
void  parseNodeData    (Node* curNode,    FILE* DBFileptr);
Node* treeCpy          (const Node* node);
Node* diff (const Node* node, char* varName);
Node* makeEasier (Node* node, int* isChanged);
Node* countConstExpr (Node* node, int* isChanged);
Node* optimizeTree (Node* node);
double findVar (const char* varName);
void insertVar (Node* node);
void fillTable (Var* table, size_t size);
Node* countFunction (Node* node);
Node* countFunctionInPoint (Node* node);
Node* countDerivative (Node* initialNode, size_t order, char* varName);

void  treeDump         (Tree* tree,       const char* str, ...);
void changeVarTable (char* varName, double varValue);
void tableDumpLatex (FILE* fileToPrint);
int factorial (int num);
void tableInsert (char* varName);
void varTablePoison ();
int nodeEquals (const Node* node, const double val);

int   findInTree       (Node* node,       const char* dataToFind);
void  treePrint        (const Node* node);
Node* treeParse        (Node* node,       FILE* DBFileptr);
void latexBegin        (FILE* fileToPrint);
void latexCompile ();
void latexEnd          (FILE* fileToPrint);

Node* getN (Node*** tokenArray);
Node* getP (Node*** tokenArray);
Node* getG (Node*** tokenArray);
Node* getE (Node*** tokenArray);
Node* getT (Node*** tokenArray);
Node* getPW (Node*** tokenArray);
Node* getUnOP (Node*** tokenArray);
Node* getVar (Node*** tokenArray);
int strEqual(const char *l, const char *r);

void getOpOrVarToken (Node*** tokenArray, char** string);
void getTokens (Node** tokenArray, char* string);
void getBOpToken (Node*** tokenArray, char** string);
void skipSpaces (char** string);

void drawPlot (double minX, double maxX, Node* function, char* varName, char* fileName);
void fullError (Node* function);
void McLaurenSeries (Node* function, size_t order, char* varName, FILE* fileToPrint);
