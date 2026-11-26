#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int n = 0;
    int a = 0;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1) return 1;
    printf("Enter value of a: ");
    if (scanf("%d", &a) != 1) return 1;

    if (n <= 0) {
        printf("Number of elements must be positive.\n");
        return 1;
    }

    int *b = malloc((size_t)n * sizeof(int));
    int *originalY = malloc((size_t)n * sizeof(int));
    int *optimizedY = malloc((size_t)n * sizeof(int));
    if (!b || !originalY || !optimizedY) {
        free(b);
        free(originalY);
        free(optimizedY);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("Enter %d elements of array b: ", n);
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &b[i]) != 1) {
            free(b);
            free(originalY);
            free(optimizedY);
            return 1;
        }
    }

    int originalMultiplications = 0;
    for (int i = 0; i < n; ++i) {
        int x = 10 * a;
        ++originalMultiplications;
        originalY[i] = x + b[i];
    }

    int x = 10 * a; // Loop-invariant expression moved outside the loop.
    int optimizedMultiplications = 1;
    for (int i = 0; i < n; ++i) {
        optimizedY[i] = x + b[i];
    }

    printf("\nOriginal loop:\n");
    printf("for (i = 0; i < n; i++) {\n");
    printf("    x = 10 * a;\n");
    printf("    y[i] = x + b[i];\n");
    printf("}\n");

    printf("\nOptimized loop after LICM:\n");
    printf("x = 10 * a;  // moved outside the loop\n");
    printf("for (i = 0; i < n; i++) {\n");
    printf("    y[i] = x + b[i];\n");
    printf("}\n");

    printf("\nOriginal y:  ");
    for (int i = 0; i < n; ++i) printf("%d ", originalY[i]);
    printf("\nOptimized y:  ");
    for (int i = 0; i < n; ++i) printf("%d ", optimizedY[i]);

    printf("\n\nMultiplications before optimization: %d", originalMultiplications);
    printf("\nMultiplications after optimization:  %d", optimizedMultiplications);
    if (memcmp(originalY, optimizedY, (size_t)n * sizeof(int)) == 0) {
        printf("\nResult: Both versions produce identical output.\n");
    } else {
        printf("\nResult: Output mismatch.\n");
    }

    free(b);
    free(originalY);
    free(optimizedY);
    return 0;
}
