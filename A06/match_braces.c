#include <stdio.h>
#include <stdlib.h>

struct brace {
    int line;
    int column;
    struct brace* next;
};

struct brace* push(int line, int column, struct brace* head) {
    struct brace* new = malloc(sizeof(struct brace));
    if (new == NULL) {
        printf("Memory allocation error\n");
        return head;
    }
    new->line = line;
    new->column = column;
    new->next = head;
    return new;
}

struct brace* pop(struct brace** head) {
    if (*head == NULL) return NULL;
    
    struct brace* temp = *head;
    *head = (*head)->next;
    return temp;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    int line = 1, column = 1;
    int matched_opening_braces[1024] = {0};
    int matched_closing_braces[1024] = {0};
    int i = 0;
    char temp;
    struct brace* head = NULL;

    while ((temp = fgetc(file)) != EOF) {
        if (temp == '\n') {
            line++;
            column = 0;
        }
        column++;

        if (temp == '{') {
            head = push(line, column, head);
        } else if (temp == '}') {
            struct brace* popped = pop(&head);
            if (popped != NULL) {
                matched_opening_braces[i] = popped->line;
                matched_closing_braces[i] = line;
                i++;
                free(popped);
            } else {
                printf("Unmatched closing brace on Line %d, Column %d\n", line, column);
                fclose(file);
                return 1;
            }
        }
    }

    if (head != NULL) {
        printf("Unmatched opening brace on Line %d, Column %d\n", head->line, head->column);
        while (head) {
            struct brace* temp = head;
            head = head->next;
            free(temp);
        }
        fclose(file);
        return 1;
    }

    printf("Matched braces: ( ");
    for (int j = 0; j < i; j++) {
        printf("%d ", matched_opening_braces[j]);
    }
    printf(") -> ( ");
    for (int j = 0; j < i; j++) {
        printf("%d ", matched_closing_braces[j]);
    }
    printf(")\n");

    fclose(file);
    return 0;
}
