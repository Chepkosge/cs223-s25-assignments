/*----------------------------------------------
 * Author: Bii
 * Date: 3/7
 * Description Program that encodes a message in a ppm file
 ---------------------------------------------*/
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "read_ppm.h"
 #include "write_ppm.h"
 
 int main(int argc, char** argv) {
     if (argc != 2) {
         printf("usage: encode <file.ppm>\n");
         return 0;
     }
 
     int w, h;
     struct ppm_pixel* pixels = read_ppm(argv[1], &w, &h);
     if (pixels == NULL) {
         printf("Unable to read file\n");
         return 0;
     }
 
     printf("Reading %s with width %d and height %d\n", argv[1], w, h);
     int max_chars = (w * h * 3) / 8; // 8 bits per character
     printf("Max number of characters in the image: %d\n", max_chars);
 
     // Get message from user
     char* message = malloc(sizeof(char)* max_chars+1);
     printf("Enter a phrase: ");
     fgets(message, sizeof(message), stdin);
     message[strcspn(message, "\n")] = '\0'; // Remove newline character
 
     int message_length = strlen(message) + 1; // Include null terminator
     if (message_length > max_chars) {
         printf("Message is too long! Max characters: %d\n", max_chars);
         free(pixels);
         return 1;
     }
 
     // Encode message into image
     int bit_index = 0;
     for (int i = 0; i < message_length; i++) {
         for (int j = 7; j >= 0; j--) { // Extract bits from each character
             int bit = (message[i] >> j) & 1;
 
             // Modify the least significant bit of pixels' color channels
             if (bit_index % 3 == 0)
                 pixels[bit_index / 3].red = (pixels[bit_index / 3].red & ~1) | bit;
             else if (bit_index % 3 == 1)
                 pixels[bit_index / 3].green = (pixels[bit_index / 3].green & ~1) | bit;
             else
                 pixels[bit_index / 3].blue = (pixels[bit_index / 3].blue & ~1) | bit;
 
             bit_index++;
         }
     }
 
     // Create output filename
     char output_filename[256];
     snprintf(output_filename, sizeof(output_filename), "%s-encoded.ppm", argv[1]);
 
     printf("Writing file %s\n", output_filename);
     write_ppm(output_filename, pixels, w, h);
 
     free(pixels);
     free(message);
     return 0;
 }
 

