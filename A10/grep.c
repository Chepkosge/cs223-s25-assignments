#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINE 1024

typedef struct {
    int thread_id;
    char *keyword;
    int num_files;
    char **files;
} thread_args_t;

void *search_files(void *arg) {
    thread_args_t *args = (thread_args_t *)arg;
    char line[MAX_LINE];

    printf("Thread %d processing %d file(s)\n", args->thread_id, args->num_files);

    for (int i = 0; i < args->num_files; i++) {
        FILE *fp = fopen(args->files[i], "r");
        if (!fp) {
            perror(args->files[i]);
            continue;
        }

        int line_number = 0;
        while (fgets(line, MAX_LINE, fp)) {
            line_number++;
            if (strstr(line, args->keyword)) {
                printf("Thread %d: [%s:%d] %s", args->thread_id, args->files[i], line_number, line);
            }
        }

        fclose(fp);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: ./grep <NumThreads> <Keyword> <Files>\n");
        return 1;
    }

    int num_threads = atoi(argv[1]);
    char *keyword = argv[2];
    int total_files = argc - 3;

    if (num_threads > total_files) {
        num_threads = total_files;  // don't over-allocate
    }

    pthread_t threads[num_threads];
    thread_args_t args[num_threads];

    // Divide files between threads
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        args[i].keyword = keyword;

        int count = 0;
        for (int j = i; j < total_files; j += num_threads) {
            count++;
        }

        args[i].files = malloc(count * sizeof(char *));
        args[i].num_files = count;

        int index = 0;
        for (int j = i; j < total_files; j += num_threads) {
            args[i].files[index++] = argv[3 + j];
        }

        pthread_create(&threads[i], NULL, search_files, &args[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        free(args[i].files);  // clean up
    }

    return 0;
}

