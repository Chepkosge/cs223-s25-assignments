/*----------------------------------------------
 * Author: Chepkosgei Bii
 * Date: 2/6/2025
 * Description: Progream that takes creates an array-like map where W is randomly places and the other spots on the map are filled with how far they are from W 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int numRows, numColumns;
    srand(time(0));

    // Get user input
    printf("Number of rows: ");
    scanf("%d", &numRows);
    printf("Number of columns: ");
    scanf("%d", &numColumns);

    // Allocate memory for 2D array
    int** map = malloc(sizeof(int*) * numRows);
    for(int i = 0; i < numRows; i++){
        map[i] = malloc(sizeof(int) * numColumns);
    }

    // Pick a random "W" location
    int wRow = rand() % numRows;
    int wColumn = rand() % numColumns;
    map[wRow][wColumn] = 0;

    // Fill the distances
    // Fill Upwards
    for(int i = wRow - 1; i >= 0; i--) {
        map[i][wColumn] = (wRow - i);
    }
    // Fill Downwards
    for(int i = wRow + 1; i < numRows; i++) {
        map[i][wColumn] = (i - wRow);
    }
    // Fill Leftwards
    for(int j = wColumn - 1; j >= 0; j--) {
        map[wRow][j] = (wColumn - j);
    }
    // Fill Rightwards
    for(int j = wColumn + 1; j < numColumns; j++) {
        map[wRow][j] = (j - wColumn);
    }

    // Fill Upper-Left Quadrant
    for(int i = wRow - 1; i >= 0; i--) {
        for(int j = wColumn - 1; j >= 0; j--) {
            map[i][j] = map[i+1][j+1] + 1;
        }
    }

    // Fill Upper-Right Quadrant
    for(int i = wRow - 1; i >= 0; i--) {
        for(int j = wColumn + 1; j < numColumns; j++) {
            map[i][j] = map[i+1][j-1] + 1;
        }
    }

    // Fill Lower-Left Quadrant
    for(int i = wRow + 1; i < numRows; i++) {
        for(int j = wColumn - 1; j >= 0; j--) {
            map[i][j] = map[i-1][j+1] + 1;
        }
    }

    // Fill Lower-Right Quadrant
    for(int i = wRow + 1; i < numRows; i++) {
        for(int j = wColumn + 1; j < numColumns; j++) {
            map[i][j] = map[i-1][j-1] + 1;
        }
    }

    // Print the map
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numColumns; j++) {
            if(map[i][j] == 0) {
                printf(" W  ");
            } else {
                printf(" %d  ", map[i][j]);
            }
        }
        printf("\n");
    }

    // Free memory
    for (int i = 0; i < numRows; i++) {
        free(map[i]);
    }
    free(map);

    return 0;
}

