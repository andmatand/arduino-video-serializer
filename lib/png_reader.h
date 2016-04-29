#include <stdlib.h>
#include <stdio.h>
#include <png.h>

typedef struct {
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers;
} coolpng;

coolpng read_png_file(char *filename);

