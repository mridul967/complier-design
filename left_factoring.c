#include <stdio.h>
#include <string.h>

int main() {
    char nonTerminal;
    char productions[20][100];
    char prefix[100];

    int n;
    int prefixLength = 0;

    printf("Enter the non-terminal: ");
    scanf(" %c", &nonTerminal);

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (without the arrow):\n");

    for (int i = 0; i < n; i++) {
        scanf("%s", productions[i]);
    }

    /*
     * Find the longest common prefix.
     */
    strcpy(prefix, productions[0]);

    for (int i = 1; i < n; i++) {
        int j = 0;

        while (prefix[j] != '\0' &&
               productions[i][j] != '\0' &&
               prefix[j] == productions[i][j]) {
            j++;
        }

        prefix[j] = '\0';
    }

    prefixLength = strlen(prefix);

    if (prefixLength == 0) {
        printf("\nNo left factoring is required.\n");
    } else {
        printf("\nGrammar after left factoring:\n");

        printf("%c -> %s%c'\n",
               nonTerminal, prefix, nonTerminal);

        printf("%c' -> ", nonTerminal);

        for (int i = 0; i < n; i++) {
            if (productions[i][prefixLength] == '\0') {
                printf("epsilon");
            } else {
                printf("%s", productions[i] + prefixLength);
            }

            if (i < n - 1)
                printf(" | ");
        }

        printf("\n");
    }

    return 0;
}