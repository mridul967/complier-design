#include <stdio.h>
#include <string.h>
int main() {
    char nonTerminal;
    char productions[20][100];
    char alpha[20][100], beta[20][100];

    int n, alphaCount = 0, betaCount = 0;

    printf("Enter the non-terminal: ");
    scanf(" %c", &nonTerminal);

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (without the arrow):\n");

    for (int i = 0; i < n; i++) {
        scanf("%s", productions[i]);

        if (productions[i][0] == nonTerminal) {
            strcpy(alpha[alphaCount], productions[i] + 1);
            alphaCount++;
        } else {
            strcpy(beta[betaCount], productions[i]);
            betaCount++;
        }
    }

    if (alphaCount == 0) {
        printf("\nThe grammar is not left recursive.\n");
    } else if (betaCount == 0) {
        printf("\nLeft recursion cannot be removed because no beta production exists.\n");
    } else {
        printf("\nGrammar after removing left recursion:\n");

        printf("%c -> ", nonTerminal);

        for (int i = 0; i < betaCount; i++) {
            printf("%s%c'", beta[i], nonTerminal);

            if (i < betaCount - 1)
                printf(" | ");
        }

        printf("\n");

        printf("%c' -> ", nonTerminal);

        for (int i = 0; i < alphaCount; i++) {
            printf("%s%c'", alpha[i], nonTerminal);

            if (i < alphaCount - 1)
                printf(" | ");
        }

        printf(" | epsilon\n");
    }

    return 0;
}
