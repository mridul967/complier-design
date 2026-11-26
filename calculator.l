%{
#include <stdio.h>
#include <stdlib.h>

int first, second;
char operator;
%}

%%
[0-9]+ {
    if (operator == 0)
        first = atoi(yytext);
    else
        second = atoi(yytext);
}

[+\-*/] {
    operator = yytext[0];
}

\n {
    switch (operator) {
        case '+':
            printf("Result: %d\n", first + second);
            break;
        case '-':
            printf("Result: %d\n", first - second);
            break;
        case '*':
            printf("Result: %d\n", first * second);
            break;
        case '/':
            if (second == 0)
                printf("Error: Division by zero\n");
            else
                printf("Result: %d\n", first / second);
            break;
        default:
            printf("Invalid expression\n");
    }

    first = 0;
    second = 0;
    operator = 0;
}

[ \t] ;

. {
    printf("Invalid character: %s\n", yytext);
}
%%

int main() {
    printf("Enter expression, for example 10 + 5:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}