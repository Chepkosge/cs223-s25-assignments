#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
void sigint_handler(int sig){
  printf("Help! I've been shot!\n");
  exit(0);
}

int main()
{
  signal(SIGINT, sigint_handler);
  while(1){

  }
  return 0;
}
