#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    const char *input;
    size_t len;
    size_t pos;
    const char *error;
} Parser;

static Node *makeNode(const char *str, size_t len, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    char *val = malloc(len + 1);
    if (!node || !val) {
        free(node);
        free(val);
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    memcpy(val, str, len);
    val[len] = '\0';
    node->value = val;
    node->left = left;
    node->right = right;
    return node;
}

static void deleteTree(Node *node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node->value);
    free(node);
}

static Node *parseExpression(Parser *p);

static Node *parseFactor(Parser *p) {
    if (p->pos >= p->len) {
        if (!p->error) p->error = "Unexpected end of expression";
        return NULL;
    }
    if (p->input[p->pos] == '(') {
        p->pos++;
        Node *node = parseExpression(p);
        if (!node) return NULL;
        if (p->pos >= p->len || p->input[p->pos] != ')') {
            deleteTree(node);
            if (!p->error) p->error = "Missing closing parenthesis";
            return NULL;
        }
        p->pos++;
        return node;
    }

    if (isalnum((unsigned char)p->input[p->pos]) || p->input[p->pos] == '_') {
        size_t start = p->pos;
        while (p->pos < p->len &&
               (isalnum((unsigned char)p->input[p->pos]) || p->input[p->pos] == '_')) {
            p->pos++;
        }
        return makeNode(p->input + start, p->pos - start, NULL, NULL);
    }

    if (!p->error) p->error = "Invalid symbol in expression";
    return NULL;
}

static Node *parseTerm(Parser *p) {
    Node *node = parseFactor(p);
    if (!node) return NULL;
    while (p->pos < p->len && (p->input[p->pos] == '*' || p->input[p->pos] == '/')) {
        char op = p->input[p->pos++];
        Node *right = parseFactor(p);
        if (!right) {
            deleteTree(node);
            return NULL;
        }
        node = makeNode(&op, 1, node, right);
    }
    return node;
}

static Node *parseExpression(Parser *p) {
    Node *node = parseTerm(p);
    if (!node) return NULL;
    while (p->pos < p->len && (p->input[p->pos] == '+' || p->input[p->pos] == '-')) {
        char op = p->input[p->pos++];
        Node *right = parseTerm(p);
        if (!right) {
            deleteTree(node);
            return NULL;
        }
        node = makeNode(&op, 1, node, right);
    }
    return node;
}

static Node *parse(Parser *p) {
    Node *root = parseExpression(p);
    if (!root) return NULL;
    if (p->pos != p->len) {
        deleteTree(root);
        if (!p->error) p->error = "Unexpected symbol after expression";
        return NULL;
    }
    return root;
}

static void printTree(const Node *node, const char *prefix, int isLast, int isRoot) {
    if (!node) return;
    if (isRoot) {
        printf("%s\n", node->value);
    } else {
        printf("%s%s%s\n", prefix, isLast ? "\\-- " : "|-- ", node->value);
    }

    if (!node->left && !node->right) return;

    size_t prefixLen = strlen(prefix);
    char *childPrefix = malloc(prefixLen + 5);
    if (!childPrefix) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    memcpy(childPrefix, prefix, prefixLen);
    if (!isRoot) {
        memcpy(childPrefix + prefixLen, isLast ? "    " : "|   ", 4);
        childPrefix[prefixLen + 4] = '\0';
    } else {
        childPrefix[prefixLen] = '\0';
    }

    if (node->left) {
        printTree(node->left, childPrefix, node->right == NULL, 0);
    }
    if (node->right) {
        printTree(node->right, childPrefix, 1, 0);
    }
    free(childPrefix);
}

int main(void) {
    // shortcut: fixed 1024-byte input buffer, switch to getline() for longer expressions
    char line[1024];
    char cleaned[1024];

    printf("Enter expression: ");
    if (!fgets(line, sizeof(line), stdin)) {
        line[0] = '\0';
    }

    size_t len = 0;
    for (size_t i = 0; line[i] != '\0'; ++i) {
        if (!isspace((unsigned char)line[i])) {
            cleaned[len++] = line[i];
        }
    }
    cleaned[len] = '\0';

    Parser parser = {cleaned, len, 0, NULL};
    Node *root = parse(&parser);
    if (!root) {
        printf("Invalid expression: %s\n", parser.error);
        return 1;
    }

    printf("\nSyntax tree:\n");
    printTree(root, "", 1, 1);
    deleteTree(root);
    return 0;
}
