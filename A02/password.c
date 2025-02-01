/***************************************************
 * Author:Chepkosgei Bii 
 * 
 */
#include <stdio.h>

int main() {
  char password[68];
  printf("Enter a password: ");
  scanf("%s", password);
  int i = 0;
  while(password[i] != '\0'){
    if(password[i] == 'e'){
      password[i] = '3';
    }
    else if(password[i] == 'a'){
      password[i] = '@';
    }
    else if(password[i] == 'l'){
      password[i] = '1';
    }
    i++;
  }
  printf("Your bad password is %s\n", password);
  return 0;
}