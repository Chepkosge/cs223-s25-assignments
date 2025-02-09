/*----------------------------------------------
 * Author: Chepkosgei Bii
 * Date: 2/7/2025
 * Description Program that gets a string and number n from user then creates and prints a string that is n times the given string
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char word[68];
    int count;

    printf("Enter a word: ");
    scanf("%s", word);
    
    printf("Enter a count: ");
    scanf("%d", &count);

    if(count >= 10000000000){
      printf("Cannot allocate new string. Exiting...");
      return 1;
    }
    else{
      int word_len = strlen(word);
    char* new_word = malloc(sizeof(char) * (word_len * count + 1));

    if (new_word == NULL) { 
        printf("Memory allocation failed!\n");
        return 1;
    }

    new_word[0] = '\0';

    for (int i = 0; i < count; i++) {
        strcat(new_word, word);
    }

    printf("%s\n", new_word);

    free(new_word); 
    return 0;
    }
}
