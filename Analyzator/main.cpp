#include <stdio.h>
#include <math.h>
#include <cassert>
#include "../diff.h"
#include <cstring>
#include <ctype.h>

#define createNum(NUM) createNode(Num_t, UnknownOp, NUM, nullptr, nullptr, nullptr)
#define dL diff(node->left, varName)
#define dR diff(node->right, varName)
#define cL treeCpy(node->left)
#define cR treeCpy(node->right)
#define SUB(left, right) createNode(OP_t, OP_SUB, 0, nullptr, left, right)
#define ADD(left, right) createNode(OP_t, OP_ADD, 0, nullptr, left, right)
#define MUL(left, right) createNode(OP_t, OP_MUL, 0, nullptr, left, right)
#define DIV(left, right) createNode(OP_t, OP_DIV, 0, nullptr, left, right)
#define POW(left, right) createNode(OP_t, OP_POW, 0, nullptr, left, right)
#define COS(right) createNode(      OP_t, OP_COS, 0, nullptr, nullptr, right)
#define SIN(right) createNode(      OP_t, OP_SIN, 0, nullptr, nullptr, right)
#define LOG(left, right) createNode(OP_t, OP_LOG, 0, nullptr, left, right)
#define LN(right) createNode(       OP_t, OP_LN,  0, nullptr, nullptr, right)
#define LBR()     createNode(       OP_t, OP_LBR, 0, nullptr, nullptr, nullptr)
#define RBR()     createNode(       OP_t, OP_RBR, 0, nullptr, nullptr, nullptr)
#define VAR(name) createNode(Var_t, UnknownOp, 0, name, nullptr, nullptr)

Node* getN (Node*** tokenArray)
{
    double val = 0;
    int isMinus = 0;
 
    if ((***tokenArray).opValue == OP_SUB)
    {
         isMinus = 1;
         *tokenArray += 1;
    }

    assert ((***tokenArray).type == Num_t);

    val = (***tokenArray).numValue;
    printf ("VAL %lf\n", val);
    *tokenArray += 1;


     if (isMinus) 
        return createNum (-val);
     else
        return createNum (val);
}

Node* getT (Node*** tokenArray)
{
    Node* val = getPW (tokenArray);

    while ((***tokenArray).opValue == OP_DIV || (***tokenArray).opValue == OP_MUL)
    {
        int op = (***tokenArray).opValue;

        *tokenArray += 1;

        Node* val2 = getPW (tokenArray);

        if (op == OP_MUL)
            val = MUL (val, val2);            
        else
            val = DIV (val, val2);
    }

    return val;
}

Node* getE (Node*** tokenArray)
{
    Node* val = getT (tokenArray);

    while ((***tokenArray).opValue == OP_ADD || (***tokenArray).opValue == OP_SUB)
    {
        int op = (***tokenArray).opValue;

        *tokenArray += 1;

        Node* val2 = getT (tokenArray);

        if (op == OP_ADD)
            val = ADD (val, val2);
        else 
            val = SUB (val, val2);
    }

    return val;
}

Node* getP (Node*** tokenArray)
{
    Node* val = nullptr;
    if ((***tokenArray).opValue == OP_LBR)
    {
        *tokenArray += 1;
        val = getE (tokenArray);
        printf ("GetP %lf\n", val);
        assert ((***tokenArray).opValue == OP_RBR);
        *tokenArray += 1;
    }
    else 
    {
       val = getUnOP (tokenArray);
    }

    return val;
}

Node* getPW (Node*** tokenArray)
{
    Node* val = getP (tokenArray);

    if (**tokenArray == nullptr) 
    {
        return val;
    }

    while ((***tokenArray).opValue == OP_POW)
    {
        *tokenArray += 1;
        val = POW (val, getP (tokenArray));

        return val;
    }

    return val;
}

Node* getVar (Node*** tokenArray)
{
    assert (tokenArray != nullptr);
    Node* val = nullptr;

    if ((***tokenArray).type == Var_t)
    {
       tableInsert ((***tokenArray).varName);
       val = VAR((***tokenArray).varName);
       *tokenArray += 1;
        
    }

    return val;
}

Node* getUnOP (Node*** tokenArray)
{
    Node* val  = nullptr;

    if ((***tokenArray).type == OP_t)
    {
        switch ((***tokenArray).opValue)
        {
            case OP_SIN:
                *tokenArray += 1;
                val = SIN (getP (tokenArray));
                printf ("SIN %p\n", val);
                break;
            
            case OP_COS:
                *tokenArray += 1;
                val = COS (getP (tokenArray));
                printf ("COS %p\n", val);
                break;
        }

    }
    else if ((***tokenArray).type == Var_t)
    {
        val = getVar (tokenArray);
    }
    else if ((***tokenArray).type == Num_t)
    {
        val = getN (tokenArray);
    }

    return val;
}
Node* getG (Node*** tokenArray) 
{
    Node* val = getE (tokenArray);
    return val;

}

int power(int base, int n)
{
    int p = 1;

    for (int i = 1; i <= n; ++i)
        p = p * base;

    return p;
}

int strEqual(const char *l, const char *r)
{
    if (l == nullptr || r == nullptr) return 0;

    for (int i = 0; l[i] != '\0' && r[i] != '\0'; i++)
        if (l[i] != r[i]) return 0;

    return 1;
}

void getBOpToken (Node*** tokenArray, char** string)
{
    switch (**string)
    {
        case '+':
            **tokenArray = ADD (nullptr, nullptr);
            *tokenArray += 1;
            break;
        
        case '-':
            **tokenArray = SUB (nullptr, nullptr);
            *tokenArray += 1;
            break;
        
        case '*':
            **tokenArray = MUL (nullptr, nullptr);
            *tokenArray += 1;;
            break;
        
        case '/':
            **tokenArray = DIV (nullptr, nullptr);
            *tokenArray += 1;
            break;
        
        case '^':
            **tokenArray = POW (nullptr, nullptr);
            *tokenArray += 1;
            break;
        
        case '(':
            **tokenArray = LBR ();
            *tokenArray += 1;
            break;
        
        case ')':
            **tokenArray = RBR ();
            *tokenArray += 1;
            break;
        

        default:
            assert (0);
    }

    *string += 1;
}

void skipSpaces (char** string)
{
    while (isspace(**string))
    {
        *string += 1;
    }
}

void getTokens (Node** tokenArray, char* string)
{
    while (*string != '\0')
    {
        if (isspace (*string))
            skipSpaces (&string);

        else if (strchr ("+-*/^()", *string))
        {
            printf ("OP\n");
            getBOpToken (&tokenArray, &string);
        }

        else if (isdigit (*string))
        {
            double number = 0;
            int numOfScanf = 0;
            sscanf (string, "%lf%n", &number, &numOfScanf);

            *tokenArray = createNum (number);
            tokenArray++;
            string += numOfScanf;
        }

        else if (isalpha (*string))
        {
            getOpOrVarToken (&tokenArray, &string);
        }

    }

    *tokenArray = nodeCtor ();
}

void getOpOrVarToken (Node*** tokenArray, char** string)
{
    char* ptr = *string;

    if (strEqual (ptr, "sin"))
    {
        *string += 3;
        **tokenArray = SIN (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "cos"))
    {
        *string += 3;
        **tokenArray = COS (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "ln"))
    {
        *string += 2;
        **tokenArray = LN (nullptr);
        *tokenArray += 1;
    }
    else if (strEqual (ptr, "log"))
    {
        *string += 3;
        **tokenArray = LOG (nullptr, nullptr);
        *tokenArray += 1;
    }
    else 
    {
        char*  data = (char*) calloc (MAXDATASIZE, sizeof(*data));
        int numOfScan = 0;

        sscanf (*string, "%[^()-*+/^]%n", data, &numOfScan);

        assert (numOfScan != NULL);
        *string += numOfScan;

        **tokenArray = VAR (data);
        *tokenArray += 1;
    }
}
