#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(void)
{
    // width, height (pixel)
    bmp_pixel a;
    a.r = 100;
    a.g = 100;
    a.b = 255;

    BMPFILE *file = bmp_init_bmp(512,512,a); // width, height, background-color
    a.r = 255;
    a.g = 255;
    a.b = 255;

    bmp_add_line(file, 100,100 , 10, 1,10, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_dot(file, 0,0,10, a); // bmp_pointer, x, y, radius(px), color

    bmp_generate_bmp(file, "test.bmp");

    return 0;

}

