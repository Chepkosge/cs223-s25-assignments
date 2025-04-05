#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t child_pid = -1;
pid_t parent_pid = -1;

void handle_parent(int sig) {
    printf("Polo [%d]\n", getpid());
    fflush(stdout);
}

void handle_child(int sig) {
    printf("Marco [%d]\n", getpid());
    fflush(stdout);
    kill(parent_pid, SIGALRM);
}

int main() {
    parent_pid = getpid();
    printf("Parent is %d\n", parent_pid);
    fflush(stdout);

    child_pid = fork();

    if (child_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        signal(SIGALRM, handle_child);
        while (1) {
            pause();  // Wait for signals
        }
    } else {
        // Parent process
        signal(SIGALRM, handle_parent);

        while (1) {
            char ch = getchar();
            if (ch == '\n') continue;

            if (ch == 'm') {
                kill(child_pid, SIGALRM);
            } else if (ch == 'q') {
                kill(child_pid, SIGTERM);
                wait(NULL);  // Wait for child to terminate
                break;
            }
        }
    }

    return 0;
}
