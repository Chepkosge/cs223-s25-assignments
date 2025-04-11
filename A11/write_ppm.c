#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pxs, int w, int h) {
    FILE* file = fopen(filename, "wb");
    if(!file){
        printf("Error: fopen failed\n");
    }
  // Write PPM header
  fprintf(file, "P6\n");
  fprintf(file, "# Overwritten by write_ppm_2d\n");
  fprintf(file, "%d %d\n", w, h);
  fprintf(file, "255\n");
  //write new pixel values
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
        fwrite(&(pxs[i][j]), sizeof(struct ppm_pixel), 1, file);
    }
  }
}
