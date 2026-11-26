#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "class",
    "public", "private", "protected", "this", "new",
    "delete", "try", "catch", "throw", "true", "false",
    "namespace", "using", "template", "virtual"
};

int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

int main() {
    char sentence[500];
    char word[50];
    int found = 0;

    printf("Enter the sentence: ");
    fgets(sentence, sizeof(sentence), stdin);

    char *token = strtok(sentence, " \t\n");

    while (token != NULL) {
        int len = strlen(token);

        // Remove punctuation from the end
        while (len > 0 && ispunct(token[len - 1]))
            token[--len] = '\0';

        // Check the word against all keywords
        for (int i = 0; i < keywordCount; i++) {
            if (strcmp(token, keywords[i]) == 0) {
                printf("Keyword found: %s\n", token);
                found = 1;
                break;
            }
        }

        token = strtok(NULL, " \t\n");
    }

    if (!found)
        printf("No C++ keywords present in this sentence.\n");

    return 0;
}