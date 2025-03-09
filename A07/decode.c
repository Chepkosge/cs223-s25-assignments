/*----------------------------------------------
 * Author: Bii
 * Date: 3/7
 * Description Program that decodes message encoded in a ppm file
 ---------------------------------------------*/
 #include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: decode <file.ppm>\n");
        return 0;
    }

    int w, h;
    struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
    if (pixels == NULL) {
        printf("Unable to read file\n");
        return 0;
    }

    printf("Reading %s with width %d and height %d\n", argv[1], w, h);
    printf("Max number of characters in the image: %d\n", (w * h * 3) / 8);

    int bit_count = 0;
    unsigned char character = 0;

    for (int i = 0; i < w * h; i++) {
        // Extract LSBs from red, green, and blue channels
        unsigned char bits[] = {
            pixels[i].red & 1,
            pixels[i].green & 1,
            pixels[i].blue & 1
        };

        for (int j = 0; j < 3; j++) {
            character = (character << 1) | bits[j]; // Shift left and add new bit
            bit_count++;

            if (bit_count == 8) { // If 8 bits collected, print character
                if (character == '\0') {
                    printf("\n");
                    free(pixels);
                    return 0;
                }
                printf("%c", character);
                bit_count = 0;
                character = 0; // Reset character
            }
        }
    }

    printf("\n");
    free(pixels);
    return 0;
}
