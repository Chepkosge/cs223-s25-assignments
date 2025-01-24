#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***************************************************
 * mathgame.c
 * Author: Chepkosgei Bii
 * Implements a math game
 **************************************************/

int main() {
    int rounds, correct = 0;

    srand(time(NULL)); 

    printf("Welcome to Math Game!\n");
    printf("How many rounds do you want to play? ");
    scanf("%d", &rounds);

    for (int i = 0; i < rounds; i++) {
        int a = rand() % 9 + 1; 
        int b = rand() % 9 + 1; 
        int answer;

        printf("%d + %d = ? ", a, b);
        scanf("%d", &answer);

        if (answer == a + b) {
            printf("Correct!\n");
            correct++;
        } else {
            printf("Incorrect :( \n");
        }
    }

    printf("You answered %d/%d correctly.\n", correct, rounds);
    return 0;
}
