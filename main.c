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

    bmp_add_line(file, 255, 255, 0, 0, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 0, 255, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 0, 511, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 127,511,255, 255, 1, a);
    bmp_add_line(file, 255, 255, 255, 511, 1, a);
    bmp_add_line(file, 255, 255, 127+255, 511, 1, a);
    bmp_add_line(file, 255,255,511, 511, 1, a);
    bmp_add_line(file, 255,255, 511, 127+255, 1, a);
    bmp_add_line(file, 255,255, 511, 255, 1, a);
    bmp_add_line(file, 255,255, 511, 127, 1, a);
    bmp_add_line(file, 255,255, 511, 0, 1, a);

    //bmp_add_line(file, 100,100 , 10, 200,1, a);
    a.r = 255;
    a.g = 0;
    a.b = 0;
    bmp_add_dot(file, 200,200,1, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(file, 100,400,2, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(file, 100,511,2, a);

    bmp_generate_bmp(file, "test.bmp");

    return 0;

}

