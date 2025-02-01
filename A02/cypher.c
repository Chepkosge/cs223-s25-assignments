/***************************************************
 * Author: Chepkosgei Bii
 * 
 */
#include <stdio.h>

int main() {
  int shift;
  char oldWord[68];
  char newWord[68];
  printf("Enter a word: ");
  scanf("%s", oldWord);
  printf("Enter a shift: ");
  scanf("%d", &shift);
  int i = 0;
  while(oldWord[i] != '\0'){
    newWord[i] = (oldWord[i] - 'a' + shift)%26 + 'a';
    i++;
  }
  newWord[i] = '\0';
  printf("Your cypher is: %s\n", newWord);
  return 0;
}
