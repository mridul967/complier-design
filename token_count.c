#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char str[1000];
    int count = 0, insideToken = 0;

    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            if (!insideToken) {
                count++;
                insideToken = 1;
            }
        } else {
            insideToken = 0;
        }
    }

    printf("Number of tokens: %d\n", count);
    return 0;
}