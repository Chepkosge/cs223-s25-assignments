#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "write_ppm.h"

struct thread_data {
  int starti;
  int endi;
  int width;
  int height;
  struct ppm_pixel color;
  struct ppm_pixel** image;
};

void *start(void* userdata) {
  struct thread_data* data = (struct thread_data*) userdata;
  // todo: your code here 
  printf("Thread is coloring rows %d to %d with color: %d %d %d\n", data->starti, data->endi, data->color.red, data->color.green, data->color.blue);
  for(int i = data->starti; i < data->endi; i++){
    for(int j = 0; j < data->width; j++){
      data->image[i][j] = data->color;
    }
  }
  return 0;
}

int main(int argc, char** argv) {

  if (argc != 2)
  {
    printf("usage: %s <num>\n", argv[0]);
    exit(0);
  }
  int N = strtol(argv[1], NULL, 10);

  int size = 1024;
  struct ppm_pixel** image = malloc(sizeof(struct ppm_pixel*) * size);
  for(int i = 0; i < size; i++){
    image[i] = malloc(sizeof(struct ppm_pixel) * size);
  }
  struct ppm_pixel* colors = malloc(sizeof(struct ppm_pixel) * N);
  srand(time(NULL));
  pthread_t* threads = malloc(sizeof(pthread_t) * N);
  struct thread_data* data = malloc(sizeof(struct thread_data) * N);

  int numPerThread = size / N;
  int rem = size % N;

  for (int i = 0; i < N; i++) {
    data[i].starti = i * numPerThread + (i < rem ? i : rem);
    data[i].endi = data[i].starti + numPerThread + (i < rem ? 1 : 0);
    data[i].width = 1024;
    data[i].height = data[i].endi - data[i].starti;
    data[i].color.red = rand() % 255;
    data[i].color.green = rand() % 255;
    data[i].color.blue = rand() % 255;
    pthread_create(&threads[i], NULL, start, &data[i]);
  }

  for (int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  write_ppm_2d("stripes.ppm", image, size, size);
  for (int i = 0; i < size; i++) {
    free(image[i]);
  }
  free(image);
  free(colors);
  free(threads);
  free(data);
}
