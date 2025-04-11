#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {

  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  FILE* file = fopen(filename, "rb");
  if(!file){
    printf("Error: fopen failed\n");
    return NULL;
  }
  //get magic number
  char magic_number[4];
  fgets(magic_number, sizeof(magic_number), file);
  if (strncmp(magic_number, "P6", 2) != 0) {
    printf("Error: Unsupported file format (must be P6)\n");
    fclose(file);
    return NULL;
  }

  //read past comment line and get width and height
  char line[256];
  while(fgets(line, sizeof(line), file)){
    if(line[0] != '#'){
      sscanf(line, "%d %d", w, h);
      break;
    }
  }
  //read past any comment line and get maxval
  int maxval;
  while(fgets(line, sizeof(line), file)){
    if(line[0] != '#'){
      sscanf(line, "%d", &maxval);
      break;
    }
  }
  //Declare and fill 2D pixel array
  struct ppm_pixel** pixels = malloc(sizeof(struct ppm_pixel*) * *h);
  for(int i = 0; i < *h; i++){
    pixels[i] = malloc(sizeof(struct ppm_pixel) * *w);
    for(int j = 0; j < *w; j++){
      fread(&(pixels[i][j]), sizeof(struct ppm_pixel), 1, file);
    }
  }
fclose(file);
return pixels;
}
