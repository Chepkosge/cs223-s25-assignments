#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE 32

int main()
{
    int n = 0;
    int boolean = 1;
    char adjective[32];

    printf("Boolean: ");
    scanf("%d", &boolean);
    printf("\n");

    printf("Number: ");
    scanf("%d", &n);
    printf("\n");

    char** adjectives = malloc(sizeof(char*) * n);
    if (adjectives == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        adjectives[i] = malloc(MAXSIZE * sizeof(char));
        if (adjectives[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for adjective %d.\n", i);
            return 1;
        }
        printf("Adjective: ");
        scanf("%s", adjectives[i]);
        printf("\n");
    }

    printf("You are the most ");
    for (int i = n - 1; i > 0; i--) {
        printf("%s, ", adjectives[i]);
    }
    printf("and %s person that I know and you know it's ", adjectives[0]);

    if (boolean) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    for (int i = 0; i < n; i++) {
        free(adjectives[i]);
    }
    free(adjectives);

    return 0;
}
