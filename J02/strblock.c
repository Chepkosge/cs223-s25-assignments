//Author: Bii & Gabby
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char word[64];
    printf("Enter string: ");
    scanf("%s", word);  
    int len = strlen(word);
    char **arr = malloc(sizeof(char *) * len);
    
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < len; i++) {
        arr[i] = malloc(sizeof(char) * (len + 1)); 
        if (arr[i] == NULL) {
            printf("Memory allocation failed!\n");
            return 1;
        }
        memset(arr[i], ' ', len); 
        arr[i][len] = '\0';  
    }

    for (int i = 0; i < len; i++) {
        arr[0][i] = word[i];       
        arr[len - 1][i] = word[i]; 
        arr[i][i] = word[i]; 
    }

    for (int i = 0; i < len; i++) {
        printf("%s\n", arr[i]);
        free(arr[i]); 
    }
    
    free(arr); 

    return 0;
}

