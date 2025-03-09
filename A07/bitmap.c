/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here
  unsigned long mask = 0x1ul << 63;
  int i = 0;
  while(mask != 0){
    unsigned long temp = img & mask;
    if(temp != 0){
      printf("@ ");
      i++;
    }
    else{
      printf("  ");
      i++;
    }
    mask = mask >> 1;
    if(i%8 == 0){
      printf("\n");
    }
  }
  return 0;
}
