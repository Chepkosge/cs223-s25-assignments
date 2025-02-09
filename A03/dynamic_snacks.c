/*----------------------------------------------
 * Author: Chepkosgei Bii
 * Date: 2/7/2025
 * Description: Dynamic snack game where users create their own snack bar by giving name, price, and quantity of snacks then the snacks are printed out
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

struct snack {
    char name[32];
    float price;
    int quantity;
};

int main() {
    int numSnacks;

    printf("Enter number of snacks: ");
    scanf("%d", &numSnacks);

    struct snack* snack_array = malloc(sizeof(struct snack) * numSnacks);
    // Input for each snack
    for (int i = 0; i < numSnacks; i++) {
        printf("Enter name: ");
        scanf("%s", snack_array[i].name); 

        printf("Enter price: ");
        scanf("%f", &snack_array[i].price);

        printf("Enter quantity: ");
        scanf("%d", &snack_array[i].quantity);
    }
    //print snacks
    for (int i = 0; i < numSnacks; i++) {
        printf("%d) %s    cost: %.2f     quantity: %d\n", 
               i, snack_array[i].name, snack_array[i].price, snack_array[i].quantity);
    }
    //free snacks
    free(snack_array);
    return 0;
}
