#include <stdio.h>
#include <string.h>

char lhs[50];
char rhs[50][50];
int ruleCount = 0;

char input[100];
int pos = 0;

int parse(char symbol) {
    int i, j, oldPos;
    int isNonTerminal = 0;

    /* Check whether symbol is a non-terminal */
    for (i = 0; i < ruleCount; i++) {
        if (lhs[i] == symbol) {
            isNonTerminal = 1;
            break;
        }
    }

    /* Terminal */
    if (!isNonTerminal) {
        if (input[pos] == symbol) {
            pos++;
            return 1;
        }
        return 0;
    }

    /* Try each production */
    for (i = 0; i < ruleCount; i++) {
        if (lhs[i] != symbol)
            continue;

        oldPos = pos;

        /* Epsilon */
        if (strcmp(rhs[i], "e") == 0)
            return 1;

        int success = 1;

        for (j = 0; rhs[i][j] != '\0'; j++) {
            if (!parse(rhs[i][j])) {
                success = 0;
                break;
            }
        }

        if (success)
            return 1;

        /* Backtrack */
        pos = oldPos;
    }

    return 0;
}

int main() {
    int n, i, j;
    char rule[100];
    char *token;

    printf("Enter number of production rules: ");
    scanf("%d", &n);

    printf("Enter productions (example: S->aSb|b):\n");

    for (i = 0; i < n; i++) {
        scanf("%s", rule);

        char left = rule[0];
        char *right = rule + 3;

        token = strtok(right, "|");

        while (token != NULL) {
            lhs[ruleCount] = left;
            strcpy(rhs[ruleCount], token);
            ruleCount++;

            token = strtok(NULL, "|");
        }
    }

    char start;
    printf("Enter start symbol: ");
    scanf(" %c", &start);

    printf("Enter input string: ");
    scanf("%s", input);

    pos = 0;

    if (parse(start) && pos == strlen(input))
        printf("String belongs to the grammar.\n");
    else
        printf("String does NOT belong to the grammar.\n");

    return 0;
}