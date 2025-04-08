#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define SIZE 100

struct t_data {
  int start;
  int end;
  int* M;
  int* u;
  int* result_threads;
};

void* multiply(void* args) {
  struct t_data* data = (struct t_data*) args;
  int start = data->start;
  int end = data->end;
  int* M = data->M;
  int* u = data->u;
  int* result_threads = data->result_threads;

  for (int i = start; i < end; i++) {
    int sum = 0;
    for (int j = 0; j < SIZE; j++) {
      sum += M[i * SIZE + j] * u[j];
    }
    result_threads[i] = sum;
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  srand(time(0));

  int M[SIZE * SIZE];
  int u[SIZE];
  int result[SIZE] = {0};           
  int result_threads[SIZE] = {0};   

  for (int i = 0; i < SIZE; i++) {
    u[i] = rand() % 10 - 5;
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      int value = rand() % 10 - 5;
      M[i * SIZE + j] = value;
    }
  }

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      result[i] += M[i * SIZE + j] * u[j];
    }
  }

  // TODO: Implement your thread solution here
  int numThreads = 4;
  int numPerThread = SIZE / numThreads;

  pthread_t* thread_array = malloc(sizeof(pthread_t) * numThreads);
  struct t_data* t_data_array = malloc(sizeof(struct t_data) * numThreads);

  for (int i = 0; i < numThreads; i++) {
    t_data_array[i].start = i * numPerThread;
    t_data_array[i].end = (i == numThreads - 1) ? SIZE : t_data_array[i].start + numPerThread;
    t_data_array[i].M = M;
    t_data_array[i].u = u;
    t_data_array[i].result_threads = result_threads;

    pthread_create(&thread_array[i], NULL, multiply, &t_data_array[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(thread_array[i], NULL);
  }

  printf("Test with %d threads\n", numThreads);
  int error = 0;
  for (int i = 0; i < SIZE; i++) {
    if (result[i] != result_threads[i]) {
      printf("Mismatch at index %d: sequential=%d, threaded=%d\n", i, result[i], result_threads[i]);
    }
    error += abs(result[i] - result_threads[i]);
  }

  printf("Error between ground truth and multithreaded version: %d\n", error);

  free(thread_array);
  free(t_data_array);

  return 0;
}
