#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t  BYTE;
    char *filename = "###.jpg";
    
    FILE *memc = fopen(argv[1],"r");
    FILE *img;
    BYTE bytes[512];
    int jpg_count = -1;
    int read_bytes;
    int null;
    
    
    do
    {
        read_bytes = fread(&bytes, 1, 512, memc);
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            jpg_count++;
            if (jpg_count == 0)
            {
                sprintf(filename, "%03i.jpg", jpg_count);
                img = fopen(filename,"w");
                null = fwrite(&bytes, 1, 512, img);
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", jpg_count);
                img = fopen(filename,"w");
                null = fwrite(&bytes, 1, 512, img);
            }
        }    
        else if (jpg_count > -1)
        {
            null = fwrite(&bytes, 1, 512, img);
        }
        
    } while (read_bytes != 0);
    
    fclose(img);
    fclose(memc);
    
}
