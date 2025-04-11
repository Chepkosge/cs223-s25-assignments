#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <getopt.h>
#include "read_ppm.h"
#include "write_ppm.h"
//barrier struct and functions(My system doesn't support barriers)
typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int count;
  int crossing;
} my_barrier_t;

void my_barrier_init(my_barrier_t *barrier, int count) {
  barrier->count = count;
  barrier->crossing = 0;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->cond, NULL);
}

void my_barrier_wait(my_barrier_t *barrier) {
  pthread_mutex_lock(&barrier->mutex);
  barrier->crossing++;
  if (barrier->crossing < barrier->count) {
      pthread_cond_wait(&barrier->cond, &barrier->mutex);
  } else {
      barrier->crossing = 0;
      pthread_cond_broadcast(&barrier->cond);
  }
  pthread_mutex_unlock(&barrier->mutex);
}
my_barrier_t barrier;
//struct for thread arguments
struct t_data{
  struct ppm_pixel** pixel;
  struct ppm_pixel** bright_pixel;
  struct ppm_pixel** blurred_pixel;
  struct ppm_pixel** glow_pixel;
  int start;
  int end;
  int width;
  int height;
};

//compute average
int find_avg(struct ppm_pixel pixel){
  int red = pixel.red;
  int blue = pixel.blue;
  int green = pixel.green;
  int avg = (red+blue+green)/3;
  return avg;
}

void* compute(void* t_args){
  struct t_data* data = (struct t_data*) t_args;
  struct ppm_pixel** pixels = data->pixel;
  struct ppm_pixel** bright_pixels = data->bright_pixel;
  struct ppm_pixel** blurred_pixels = data->blurred_pixel;
  struct ppm_pixel** glow_pixels = data->glow_pixel;
  int start = data->start;
  int end = data->end;
  int width = data->width;
  int height = data->height;
  //find bright pixels from given rows
  int num_rows = end - start;
  for(int i = start; i < end; i++){
    for(int j = 0; j < width; j++){
      int avg = find_avg(pixels[i][j]);
      if(avg > 200){
        bright_pixels[i][j] = pixels[i][j];
      } 
      else{
        bright_pixels[i][j].red = 0;
        bright_pixels[i][j].green = 0;
        bright_pixels[i][j].blue = 0;
      }
    }
  }
  my_barrier_wait(&barrier);

  //blur
  for(int i = start; i < end; i++){
    for(int j = 0; j < width; j++){
      int avg = find_avg(bright_pixels[i][j]);
      int new_red = 0;
      int new_blue = 0;
      int new_green = 0;
      if(avg != 0){
        int m;
        int f;
        if(j == 0){
          m = j;
          f = j+1;
        }
        else if(j == width-1){
          m = j-1;
          f = j;
        }
        else{
          m = j-1;
          f = j+1;
        }
        for(int mm = m; mm <= f; mm++){
          int n;
          int z;
          if(i == 0){
            n = i;
            z = i+1;
          }
          else if(i == height-1){
            n = i-1;
            z = i;
          }
          else{
            n = i-1;
            z = i+1;
          }
          for(int nn = n; nn <= z; nn++){
            new_red += bright_pixels[nn][mm].red;
            new_blue += bright_pixels[nn][mm].blue;
            new_green += bright_pixels[nn][mm].green;       
          }
        }
        blurred_pixels[i][j].red = new_red / 9;
        blurred_pixels[i][j].blue = new_blue / 9;
        blurred_pixels[i][j].green = new_green / 9;
      }
    }
  }
  my_barrier_wait(&barrier);
//add blurred to original to make glow
for(int i = start; i < end; i++){
  for(int j = 0; j < width; j++){
    int red_sum = pixels[i][j].red + blurred_pixels[i][j].red;
    int blue_sum = pixels[i][j].blue + blurred_pixels[i][j].blue;
    int green_sum = pixels[i][j].green + blurred_pixels[i][j].green;

    glow_pixels[i][j].red = red_sum > 255 ? 255 : red_sum;
    glow_pixels[i][j].blue = blue_sum > 255 ? 255 : blue_sum;
    glow_pixels[i][j].green = green_sum > 255 ? 255 : green_sum;
  }
}
  return NULL;
}

int main(int argc, char* argv[]) 
{

  int opt;
  int N = 4;
  int threshold = 200;
  int blursize = 24;
  const char* filename = "earth-small.ppm";
  int width;
  int height;
  while ((opt = getopt(argc, argv, ":N:t:b:f:")) != -1) {
    switch (opt) {
      case 'N': N = atoi(optarg); break;
      case 't': threshold = atof(optarg); break;
      case 'b': blursize = atof(optarg); break;
      case 'f': filename = optarg; break;
      case 'h':
        printf("usage: %s -N <NumThreads> -t <brightness threshold> -b <box blur size> -f <ppmfile>\n", argv[0]);
        exit(0);
      case '?': 
        fprintf(stderr, "Unknown option: -%c\n", optopt);
        printf("usage: %s -N <NumThreads> -t <brightness threshold> -b <box blur size> -f <ppmfile>\n", argv[0]); break;
        
    }
  }
  // todo: your code here
  // compute image
  //read pixels
  struct ppm_pixel** pixels = read_ppm_2d(filename, &width, &height);
  struct ppm_pixel** bright_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  struct ppm_pixel** blurred_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  struct ppm_pixel** glow_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  for(int i = 0; i < height; i++){
    bright_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
    blurred_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
    glow_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
  }
  my_barrier_init(&barrier, N);
  int numPerThread = height / N;
  int rem = height % N;
  pthread_t* thread_array = malloc(sizeof(pthread_t) * N);
  struct t_data* t_data_array = malloc(sizeof(struct t_data) * N);
  for(int i = 0; i < N; i++){
    t_data_array[i].pixel = pixels;
    t_data_array[i].bright_pixel = bright_pixels;
    t_data_array[i].blurred_pixel = blurred_pixels;
    t_data_array[i].glow_pixel = glow_pixels;
    t_data_array[i].start = i * numPerThread + (i < rem ? i : rem);
    t_data_array[i].end = t_data_array[i].start + numPerThread + (i < rem ? 1 : 0);
    t_data_array[i].width = width;
    t_data_array[i].height = height;
    printf("Thread %d: %d - %d\n", i, t_data_array[i].start, t_data_array[i].end);
    pthread_create(&thread_array[i], NULL, compute, &t_data_array[i]);
  }

  for(int i = 0; i < N; i++){
    pthread_join(thread_array[i], NULL);
  }
  //overwrite earth.ppm with the glowing version
  write_ppm_2d("earth3.ppm", glow_pixels, width, height);

  for(int i = 0; i < height; i++){
    free(pixels[i]);
    free(bright_pixels[i]);
    free(blurred_pixels[i]);
    free(glow_pixels[i]);
  }
  free(pixels);
  free(bright_pixels);
  free(blurred_pixels);
  free(glow_pixels);
}
