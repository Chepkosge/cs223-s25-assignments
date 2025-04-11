#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
//compute average
int find_avg(struct ppm_pixel pixel){
  int red = pixel.red;
  int blue = pixel.blue;
  int green = pixel.green;
  int avg = (red+blue+green)/3;
  return avg;
}

int main(int argc, char* argv[]) {
  struct timeval start, end;
  gettimeofday(&start, NULL);

  // todo: your code here
  int width;
  int height;
  struct ppm_pixel** pixels = read_ppm_2d("earth.ppm", &width, &height);

  //compute image
  struct ppm_pixel** bright_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  for(int i = 0; i < height; i++){
    bright_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
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
  //blur
  struct ppm_pixel** blurred_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  for(int i = 0; i < height; i++){
    blurred_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
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
  //add blurred to original to make glow
  struct ppm_pixel** glow_pixels = malloc(sizeof(struct ppm_pixel*) * height);
  for(int i = 0; i < height; i++){
    glow_pixels[i] = malloc(sizeof(struct ppm_pixel) * width);
    for(int j = 0; j < width; j++){
      int red_sum = pixels[i][j].red + blurred_pixels[i][j].red;
      int blue_sum = pixels[i][j].blue + blurred_pixels[i][j].blue;
      int green_sum = pixels[i][j].green + blurred_pixels[i][j].green;

      glow_pixels[i][j].red = red_sum > 255 ? 255 : red_sum;
      glow_pixels[i][j].blue = blue_sum > 255 ? 255 : blue_sum;
      glow_pixels[i][j].green = green_sum > 255 ? 255 : green_sum;
    }
  }
  //overwrite
  write_ppm_2d("earth2.ppm", glow_pixels, width, height);
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

  gettimeofday(&end, NULL);
  double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  printf("Elapsed time: %.6f seconds\n", elapsed_time);

  return 0;
}
