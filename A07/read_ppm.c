/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
  FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("fopen failed");
        return NULL;
    }
    
    char magic[3];
    if (!fgets(magic, sizeof(magic), file) || strcmp(magic, "P6") != 0) {
        fprintf(stderr, "Error: Invalid PPM file format\n");
        fclose(file);
        return NULL;
    }
    // Skip comment line if present
    // Skip comment lines properly
    char c;

    // Skip whitespace (spaces, newlines, tabs)
    do {
        c = fgetc(file);
    } while (isspace(c)); // Use isspace() to catch all whitespace

    // If we find a comment, keep skipping until a new line starts
    while (c == '#') { 
        while (fgetc(file) != '\n');  // Skip the rest of the comment line

        // Read next non-whitespace character
        do {
            c = fgetc(file);
        } while (isspace(c)); 
    }

    // Put back the first valid character (should be a digit for width)
    ungetc(c, file);

    // Read width and height
    if (fscanf(file, " %d %d", width, height) != 2) {
        fprintf(stderr, "Error: Invalid width/height\n");
        fclose(file);
        return NULL;
    }
    
    int maxval;
    if (fscanf(file, "%d", &maxval) != 1 || maxval > 255) {
        fprintf(stderr, "Error: Invalid max color value\n");
        fclose(file);
        return NULL;
    }
    
    fgetc(file); // Read the single whitespace after maxval
    
    // Allocate memory for pixels
    struct ppm_pixel* pixels = malloc((*width) * (*height) * sizeof(struct ppm_pixel));
    if (!pixels) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    // Read pixel data
    if (fread(pixels, sizeof(struct ppm_pixel), (*width) * (*height), file) != (*width) * (*height)) {
        fprintf(stderr, "Error: Failed to read pixel data\n");
        free(pixels);
        fclose(file);
        return NULL;
    }
    
    fclose(file);
    return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
