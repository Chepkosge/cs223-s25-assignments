//Authors: Lola & Bii
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
  int num = atoi(argv[1]);
  //Print hexadecimal
  printf("0x ");
  unsigned int mask2 = 0xF0000000;
  int j = 0;
  while (mask2 != 0x00000000){
    if((num & mask2) == 0x00000000){
      printf("0");
      j++;
    }
    else{
      unsigned int temp_num = (num & mask2) >> (28-(4*j));
      printf("%X", temp_num);
      j++;
    }
    mask2 = mask2 >> 4;
    if(j%2 == 0){
      printf(" ");
    }
  }
  printf("\n");
  //Print binary
  unsigned int mask = 0b10000000000000000000000000000000;
  printf("0b ");
  int i = 0;
  while (mask != 0b00000000000000000000000000000000){
    if((num & mask) == 0){
      printf("0");
      i++;
    } 
    else{
      printf("1");
      i++;
    }
    if(i % 4 == 0){
      printf(" ");
    }
    mask = mask >> 1;
  }
  printf("\n");
  return 0;
}
