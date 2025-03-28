#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("%d] A0\n", getpid());
    
    if (fork() == 0) { // First child process (B1)
        printf("%d] B1\n", getpid());
        if(fork()){
          printf("%d] Bye\n", getpid());
        }
    } else {
        printf("%d] B0\n", getpid());
        
        if (fork() == 0) { // Second child process (C1)
            printf("%d] C1\n", getpid());
            if(fork()){
              printf("%d] Bye\n", getpid());
            }
        } else {
            printf("%d] C0\n", getpid());
            if(fork()){
              printf("%d] Bye\n", getpid());
            }
        }
    }
    return 0;
}
