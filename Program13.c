#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10
// shortcut: 63-char max for names/types, switch to dynamic allocation if longer identifiers are needed
#define MAX_STR 64

typedef struct Symbol {
    char name[MAX_STR];
    char dataType[MAX_STR];
    struct Symbol *next;
} Symbol;

static int hashFunction(const char *name) {
    unsigned long sum = 0;
    for (size_t i = 0; name[i] != '\0'; ++i) {
        sum += (unsigned char)name[i];
    }
    return (int)(sum % TABLE_SIZE);
}

static void insertSymbol(Symbol *table[], const char *name, const char *dataType) {
    int index = hashFunction(name);
    Symbol **pp = &table[index];
    while (*pp != NULL) {
        if (strcmp((*pp)->name, name) == 0) {
            printf("Identifier already exists.\n");
            return;
        }
        pp = &(*pp)->next;
    }

    Symbol *node = malloc(sizeof(Symbol));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    snprintf(node->name, MAX_STR, "%s", name);
    snprintf(node->dataType, MAX_STR, "%s", dataType);
    node->next = NULL;
    *pp = node;
    printf("Identifier inserted successfully at bucket %d.\n", index);
}

static void lookupSymbol(Symbol *table[], const char *name) {
    int index = hashFunction(name);
    for (Symbol *cur = table[index]; cur != NULL; cur = cur->next) {
        if (strcmp(cur->name, name) == 0) {
            printf("Identifier found: %s, Data type: %s\n", cur->name, cur->dataType);
            return;
        }
    }
    printf("Identifier not found.\n");
}

static void deleteSymbol(Symbol *table[], const char *name) {
    int index = hashFunction(name);
    Symbol **pp = &table[index];
    while (*pp != NULL) {
        if (strcmp((*pp)->name, name) == 0) {
            Symbol *target = *pp;
            *pp = target->next;
            free(target);
            printf("Identifier deleted successfully.\n");
            return;
        }
        pp = &(*pp)->next;
    }
    printf("Identifier not found.\n");
}

static void displayTable(Symbol *table[]) {
    printf("\nHASH TABLE CONTENTS:\n");
    for (int i = 0; i < TABLE_SIZE; ++i) {
        printf("Bucket %d: ", i);
        if (table[i] == NULL) {
            printf("Empty");
        } else {
            for (Symbol *cur = table[i]; cur != NULL; cur = cur->next) {
                printf("[%s, %s] ", cur->name, cur->dataType);
            }
        }
        printf("\n");
    }
}

static void freeTable(Symbol *table[]) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Symbol *cur = table[i];
        while (cur != NULL) {
            Symbol *next = cur->next;
            free(cur);
            cur = next;
        }
        table[i] = NULL;
    }
}

int main(void) {
    Symbol *table[TABLE_SIZE] = {NULL};
    int choice = 0;
    char name[MAX_STR];
    char dataType[MAX_STR];

    do {
        printf("\n--- SYMBOL TABLE ---\n");
        printf("1. Insert\n2. Lookup\n3. Delete\n4. Display\n5. Exit\n");
        printf("Enter your choice: ");

        int rc = scanf("%d", &choice);
        if (rc == EOF) break;
        if (rc != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            choice = 0;
        }

        switch (choice) {
            case 1:
                printf("Enter identifier name: ");
                if (scanf("%63s", name) != 1) break;
                printf("Enter data type: ");
                if (scanf("%63s", dataType) != 1) break;
                insertSymbol(table, name, dataType);
                break;
            case 2:
                printf("Enter identifier to search: ");
                if (scanf("%63s", name) != 1) break;
                lookupSymbol(table, name);
                break;
            case 3:
                printf("Enter identifier to delete: ");
                if (scanf("%63s", name) != 1) break;
                deleteSymbol(table, name);
                break;
            case 4:
                displayTable(table);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    freeTable(table);
    return 0;
}
