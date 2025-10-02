#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

// Function to get precedence
int prec(char op)
{
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return 0;
}

// Apply operator
int calculate(int a, int b, char op, int *err)
{
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/')
    {
        if (b == 0)
        {
            *err = 1; // division by zero
            return 0;
        }
        return a / b;
    }
    return 0;
}

int evaluate(char exp[], int *err)
{
    int num[100], topNum = -1;
    char op[100];
    int topOp = -1;

    for (int i = 0; exp[i] != '\0' ; i++)
    {
        if (exp[i] == ' ')
            continue; // ignore spacesS

        if (isdigit(exp[i]))
        {
            int val = 0;
            while (i < strlen(exp) && isdigit(exp[i]))
            {
                val = val * 10 + (exp[i] - '0');
                i++;
            }
            i--;
            num[++topNum] = val;
        }
        else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/')
        {
            while (topOp != -1 && prec(op[topOp]) >= prec(exp[i]))
            {
                int b = num[topNum--];
                int a = num[topNum--];
                char oper = op[topOp--];
                num[++topNum] = calculate(a, b, oper, err);
                if (*err)
                    return 0;
            }
            op[++topOp] = exp[i];
        }
        else
        {
            *err = 2; // invalid char
            return 0;
        }
    }

    while (topOp != -1)
    {
        int b = num[topNum--];
        int a = num[topNum--];
        char oper = op[topOp--];
        num[++topNum] = calculate(a, b, oper, err);
        if (*err)
            return 0;
    }

    return num[topNum];
}

int main()
{
    char exp[MAX];
    printf("Enter expression: ");
    scanf("%[^\n]", &exp);

    int err = 0;
    int ans = evaluate(exp, &err);

    if (err == 1){
        printf("Error: Division by zero.\n");
    }
    else if(err == 2) {
        printf("Error: Invalid expression.\n");
    }
    else{
        printf("%d\n", ans);
    }

    return 0;
}
