#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX_LINE_LENGTH 1024
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Function to search for a keyword in a file
int search_file(const char *filename, const char *keyword) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int match_count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, keyword)) {
            match_count++;
            printf("%s[Match]%s %s found in %s: %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, keyword, filename, line);

        }
    }

    fclose(file);
    return match_count;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <NumProcesses> <Keyword> <Files...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read command-line arguments
    int num_processes = atoi(argv[1]);
    char *keyword = argv[2];
    int num_files = argc - 3;
    char **files = &argv[3];

    printf("Searching %d files for keyword: %s\n", num_files, keyword);

    // Start timing
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int total_matches = 0;
    int files_per_process = num_files / num_processes;
    int remaining_files = num_files % num_processes;

    // Create child processes
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // Child process
            int start_idx = i * files_per_process + (i < remaining_files ? i : remaining_files);
            int end_idx = start_idx + files_per_process + (i < remaining_files ? 1 : 0);

            int local_match_count = 0;
            printf("Process [%d] searching files (%d to %d)\n", getpid(), start_idx, end_idx - 1);

            for (int j = start_idx; j < end_idx; j++) {
                int count = search_file(files[j], keyword);
                if (count > 0) {
                    local_match_count += count;
                }
            }

            exit(local_match_count);  // Return match count as exit status
        }
    }

    // Parent process waits for children and adds results
    for (int i = 0; i < num_processes; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            total_matches += WEXITSTATUS(status);
        }
    }

    // End timing
    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Total occurrences: %d\n", total_matches);
    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    return EXIT_SUCCESS;
}
