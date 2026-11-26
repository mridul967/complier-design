#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_TOKENS 256
// shortcut: fixed 256-token / 64-char limits, switch to dynamic arrays for huge expressions
#define MAX_TOK_LEN 64

static int isOperator(const char *token) {
    return token[0] != '\0' && token[1] == '\0' &&
           (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/');
}

static int precedence(const char *op) {
    if (op[0] != '\0' && op[1] == '\0') {
        if (op[0] == '*' || op[0] == '/') return 2;
        if (op[0] == '+' || op[0] == '-') return 1;
    }
    return 0;
}

static int tokenize(const char *text, char tokens[][MAX_TOK_LEN], int maxTokens) {
    int count = 0;
    for (size_t i = 0; text[i] != '\0' && count < maxTokens;) {
        if (isspace((unsigned char)text[i])) {
            ++i;
        } else if (isalnum((unsigned char)text[i]) || text[i] == '_') {
            size_t start = i;
            while (text[i] != '\0' &&
                   (isalnum((unsigned char)text[i]) || text[i] == '_')) {
                ++i;
            }
            size_t len = i - start;
            if (len >= MAX_TOK_LEN) len = MAX_TOK_LEN - 1;
            memcpy(tokens[count], text + start, len);
            tokens[count][len] = '\0';
            ++count;
        } else {
            tokens[count][0] = text[i++];
            tokens[count][1] = '\0';
            ++count;
        }
    }
    return count;
}

static int toPostfix(char tokens[][MAX_TOK_LEN], int n, char output[][MAX_TOK_LEN], int maxOut) {
    char operators[MAX_TOKENS][MAX_TOK_LEN];
    int opTop = -1;
    int outCount = 0;

    for (int i = 0; i < n; ++i) {
        const char *token = tokens[i];
        if (!isOperator(token) && strcmp(token, "(") != 0 && strcmp(token, ")") != 0) {
            if (outCount < maxOut) strcpy(output[outCount++], token);
        } else if (strcmp(token, "(") == 0) {
            if (opTop + 1 < MAX_TOKENS) strcpy(operators[++opTop], token);
        } else if (strcmp(token, ")") == 0) {
            while (opTop >= 0 && strcmp(operators[opTop], "(") != 0) {
                if (outCount < maxOut) strcpy(output[outCount++], operators[opTop]);
                --opTop;
            }
            if (opTop >= 0) --opTop;
        } else {
            while (opTop >= 0 && isOperator(operators[opTop]) &&
                   precedence(operators[opTop]) >= precedence(token)) {
                if (outCount < maxOut) strcpy(output[outCount++], operators[opTop]);
                --opTop;
            }
            if (opTop + 1 < MAX_TOKENS) strcpy(operators[++opTop], token);
        }
    }
    while (opTop >= 0) {
        if (outCount < maxOut) strcpy(output[outCount++], operators[opTop]);
        --opTop;
    }
    return outCount;
}

static void generateCode(const char *expression) {
    char target[256] = "";
    const char *rhs = expression;
    const char *equalPos = strchr(expression, '=');

    if (equalPos != NULL) {
        size_t len = (size_t)(equalPos - expression);
        if (len >= sizeof(target)) len = sizeof(target) - 1;
        memcpy(target, expression, len);
        target[len] = '\0';
        while (len > 0 && isspace((unsigned char)target[len - 1])) {
            target[--len] = '\0';
        }
        rhs = equalPos + 1;
    }

    char tokens[MAX_TOKENS][MAX_TOK_LEN];
    char postfix[MAX_TOKENS][MAX_TOK_LEN];
    int numTokens = tokenize(rhs, tokens, MAX_TOKENS);
    int numPostfix = toPostfix(tokens, numTokens, postfix, MAX_TOKENS);

    char operands[MAX_TOKENS][MAX_TOK_LEN];
    int top = -1;
    int tempCount = 1;
    printf("Three-address code:\n");

    for (int i = 0; i < numPostfix; ++i) {
        const char *token = postfix[i];
        if (!isOperator(token)) {
            if (top + 1 < MAX_TOKENS) strcpy(operands[++top], token);
        } else {
            if (top < 1) {
                printf("Invalid expression.\n");
                return;
            }
            char right[MAX_TOK_LEN];
            char left[MAX_TOK_LEN];
            char temp[MAX_TOK_LEN];
            strcpy(right, operands[top--]);
            strcpy(left, operands[top--]);
            snprintf(temp, sizeof(temp), "t%d", tempCount++);
            printf("%s = %s %s %s\n", temp, left, token, right);
            strcpy(operands[++top], temp);
        }
    }

    if (top != 0) {
        printf("Invalid expression.\n");
        return;
    }
    if (target[0] != '\0') {
        printf("%s = %s\n", target, operands[top]);
    } else {
        printf("Final result is stored in %s\n", operands[top]);
    }
}

int main(void) {
    printf("Expression 1: x = (y + z) / k\n");
    generateCode("x = (y + z) / k");
    printf("\nExpression 2: (a + b) * (c - d)\n");
    generateCode("(a + b) * (c - d)");
    return 0;
}
