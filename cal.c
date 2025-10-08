#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#define MAX_NUM_STACK 100
#define MAX_OP_STACK 100
#define MAX_EXPRESSION 200

// ----- Stacks -----
int numberStack[MAX_NUM_STACK];
int numStackTop = -1;

char operatorStack[MAX_OP_STACK];
int opStackTop = -1;


void resetStacks()
{
    numStackTop = -1;
    opStackTop = -1;
}


void pushNumber(int value)
{
    if (numStackTop < MAX_NUM_STACK - 1)
        numberStack[++numStackTop] = value;
}

int popNumber()
{
    if (numStackTop < 0){
        return INT_MIN; // stack underflow indicator
    }
    return numberStack[numStackTop--];
}


void pushOperator(char op)
{
    if (opStackTop < MAX_OP_STACK - 1)
        operatorStack[++opStackTop] = op;
}

char popOperator()
{
    if (opStackTop < 0){
        return 0; // empty stack indicator
    }
    return operatorStack[opStackTop--];
}

// ----- Check if character is operator -----
int isOperatorChar(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}


int getOperatorPrecedence(char op)
{
    if (op == '*' || op == '/'){
        return 2;
    }
    if (op == '+' || op == '-'){
        return 1;
    }
    return 0;
}

// ----- Apply operator with overflow check -----
int applyOperator(int a, char op, int b, int *errorCode)
{
    long long result = 0;

    switch (op)
    {
    case '+':
        result = (long long)a + (long long)b;
        break;

    case '-':
        result = (long long)a - (long long)b;
        break;

    case '*':
        result = (long long)a * (long long)b;
        break;

    case '/':
        if (b == 0)
        {
            *errorCode = 2;
            return 0;
        } // division by zero
        result = a / b;
        break;

    default:
        *errorCode = 3;
        return 0; // invalid operator
    }

    if (result > INT_MAX || result < INT_MIN)
    {
        *errorCode = 4; // overflow
        return 0;
    }

    return (int)result;
}


int processTopOperator(int *errorCode)
{
    int b = popNumber();
    int a = popNumber();
    char op = popOperator();

    if (a == INT_MIN || b == INT_MIN)
    {
        *errorCode = 1;
        return 0;
    } // stack underflow
    return applyOperator(a, op, b, errorCode);
}

// ----- Evaluate arithmetic expression -----
int evaluateExpression(const char expr[], int *errorCode)
{
    char buffer[50];
    int bufIndex;

    int len = strlen(expr);
    for (int i = 0; i < len; i++)
    {
        char ch = expr[i];

        if (isdigit((unsigned char)ch))
        {
            bufIndex = 0;
            while (i < len && isdigit((unsigned char)expr[i]))
            {
                buffer[bufIndex++] = expr[i++];
            }
            buffer[bufIndex] = '\0';
            pushNumber(atoi(buffer));
            i--; // step back after number
        }
        else if (isspace((unsigned char)ch))
        {
            continue;
        }
        else if (isOperatorChar(ch))
        {
            while (opStackTop != -1 &&
                   getOperatorPrecedence(operatorStack[opStackTop]) >= getOperatorPrecedence(ch))
            {
                int res = processTopOperator(errorCode);
                if (*errorCode != 0){
                    return 0;
                }
                pushNumber(res);
            }
            pushOperator(ch);
        }
        else if (isalpha((unsigned char)ch))
        {
            *errorCode = 5;
            return 0; // contains alphabets
        }
        else
        {
            *errorCode = 6;
            return 0; // invalid symbol
        }
    }

    // Remaining operators
    while (opStackTop != -1)
    {
        int res = processTopOperator(errorCode);
        if (*errorCode != 0){
            return 0;
        }
        pushNumber(res);
    }

    int finalResult = popNumber();
    if (finalResult == INT_MIN)
    {
        *errorCode = 1;
        return 0;
    } // stack underflow

    return finalResult;
}


int main()
{
    char expression[MAX_EXPRESSION];

    while (1)
    {
        printf("\nEnter an arithmetic expression (or type 'exit' to quit): ");
        if (!fgets(expression, sizeof(expression), stdin))
            break;

        // Remove newline
        expression[strcspn(expression, "\n")] = 0;

        if (strncmp(expression, "exit", 4) == 0){
            break;
        }


        resetStacks();
        

        int errorCode = 0;
        int result = evaluateExpression(expression, &errorCode);

        switch (errorCode)
        {
        case 1:
            printf("Error: Stack underflow (insufficient operands)\n");
            break;

        case 2:
            printf("Error: Division by zero!\n");
            break;

        case 3:
            printf("Error: Invalid operator!\n");
            break;

        case 4:
            printf("Error: Integer overflow/underflow!\n");
            break;

        case 5:
            printf("Error: Expression contains alphabets!\n");
            break;

        case 6:
            printf("Error: Invalid symbol found!\n");
            break;

        default:
            printf("Result = %d\n", result);
        }
    }

    printf("Exiting program.\n");
    return 0;
}
