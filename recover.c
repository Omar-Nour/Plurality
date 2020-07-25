#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    char *filename = malloc(8 * sizeof(char));

    // exit if no file
    if (argc != 2)
    {
        return argc;
    }

    // open memory card
    FILE *memc = fopen(argv[1], "r");
    FILE *img;
    BYTE bytes[512];

    // images found counter
    int jpg_count = -1;
    int read_bytes;

    // to recieve return of fwrite
    int null;
    
    bool exit = false;


    do
    {
        read_bytes = fread(bytes, 1, 512, memc);
        exit = read_bytes == 0;
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0 && !exit)
        {
            jpg_count++;
            if (jpg_count == 0)
            {
                sprintf(filename, "%03i.jpg", jpg_count);
                img = fopen(filename, "w");
                null = fwrite(bytes, 1, 512, img);
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", jpg_count);
                img = fopen(filename, "w");
                null = fwrite(bytes, 1, 512, img);
            }
        }
        // continue to fill next blocks of previous/opened image
        else if (jpg_count > -1 && !exit)
        {
            null = fwrite(bytes, 1, 512, img);
        }

    }
    while (!exit);

    free(filename);

    // close img only if it was open asasan
    if (jpg_count > -1)
    {
        fclose(img);
    }
    fclose(memc);

}
