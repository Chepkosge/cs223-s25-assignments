/***************************************************
 * Author: Chepkosgei Bii
 * 
 */
#include <stdio.h>
#include <string.h>

struct snack{
  char name[68];
  float price;
  int quantity;
};


int main() {
  struct snack snack1, snack2, snack3;
  strcpy(snack1.name,"Cup cake");
  snack1.price = 4.0;
  snack1.quantity = 2;

  strcpy(snack2.name,"Croissant");
  snack2.price = 3.6;
  snack2.quantity = 1;

  strcpy(snack3.name,"Fries");
  snack3.price = 15.0;
  snack3.quantity = 1;

  struct snack snacks[3] = {snack1, snack2, snack3};
  printf("Welcome to Steven Struct's snack bar\n");
  float money;
  printf("How much money do you have? ");
  scanf("%f", &money);
  printf("\n");
  for(int i = 0; i < 3; i++){
    printf("%d) %s   price: $%.2f   quantity: %d\n", i, snacks[i].name, snacks[i].price, snacks[i].quantity);
  }
    printf("What snack would you like to buy? [0,1,2]\n");
    int choice;
    scanf("%d", &choice);
    if(snacks[choice].quantity == 0){
      printf("Sorry we're out of %s\n", snacks[choice].name);
    }
    else{
      if(snacks[choice].price > money){
        printf("You can't afford it!\n");
      }
      else{
        printf("You bought: %s\n", snacks[choice].name);
        printf("You have: %.2f left\n", money-snacks[choice].price);
      }
    }
  return 0;
}
