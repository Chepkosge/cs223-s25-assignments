#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

    FILE* fp = fopen(argv[1], "r");

    int shift = 1; 
    int c;

    while ((c = fgetc(fp)) != EOF)
    {
        unsigned char decoded = (c >> shift) & 0x7F; 
        printf("%c", decoded);
    }

    printf("\n");
    fclose(fp);
    return 0;
}

