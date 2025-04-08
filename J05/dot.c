#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100000

int numThreads;
int numPerThread;
int v[SIZE];
int u[SIZE];

struct t_data {
    int start;
    int end;
};

void* dot_product(void* arg) {
    int* sum = malloc(sizeof(int));
    *sum = 0;
    struct t_data* data = (struct t_data*) arg;
    for (int i = data->start; i < data->end; i++) {
        *sum += v[i] * u[i]; 
    }
    return sum;
}

int main() {
    srand(time(0));

    int dotproduct = 0;
    for (int i = 0; i < SIZE; i++) {
        v[i] = rand() % 1000 - 500;
        u[i] = rand() % 1000 - 500;
        dotproduct += u[i] * v[i];
    }
    printf("Ground truth dot product: %d\n", dotproduct);

    numThreads = 4;
    numPerThread = SIZE / numThreads;

    pthread_t* thread_array = malloc(sizeof(pthread_t) * numThreads);
    struct t_data* t_data_array = malloc(sizeof(struct t_data) * numThreads);

    int thread_dotproduct = 0;
    for (int i = 0; i < numThreads; i++) {
        t_data_array[i].start = i * numPerThread;
        t_data_array[i].end = (i == numThreads - 1) ? SIZE : t_data_array[i].start + numPerThread; // handle last chunk
        pthread_create(&thread_array[i], NULL, dot_product, &t_data_array[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        void* retval;
        pthread_join(thread_array[i], &retval);
        int* sum = (int*) retval;
        thread_dotproduct += *sum;
        free(sum);
    }

    printf("Test with 4 threads\n");
    printf("Answer with threads: %d\n", thread_dotproduct);

    free(thread_array);
    free(t_data_array);

    return 0;
}

