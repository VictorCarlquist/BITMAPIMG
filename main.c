#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

int main(void)
{
    // width, height (pixel)
    int i,j;
    bmp_pixel a;
    a.r = 255;
    a.g = 255;
    a.b = 255;
    bmp_pixel c;
    c.r = 255;
    c.g = 0;
    c.b = 0;

    BMPFILE *file = bmp_init_bmp(1024,1024,a, BMP_SCALE_NO); // width, height, background-color
    a.r = 0;
    a.g = 255;
    a.b = 255;


    bmp_add_line(file, 0,10 , 0, 200, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_dot(file, 0,10, 3, c);
    bmp_add_line(file, 0, 255, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_dot(file, 0, 255, 3, c);
    bmp_add_line(file, 0, 511, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 127,511,255, 255, 1, a);
    bmp_add_line(file, 255, 255, 255, 511, 1, a);
    bmp_add_line(file, 255, 255, 127+255, 511, 1, a);
    bmp_add_line(file, 255,255,511, 511, 1, a);
    bmp_add_line(file, 511, 127+255,255,255,  1, a);
    bmp_add_dot(file, 511, 127+255, 3, c);
    bmp_add_dot(file, 255, 255, 3, c);
    bmp_add_line(file, 511, 127+255,255,255,  1, a);
    bmp_add_line(file, 255,255, 511, 127+255, 1, a);
    bmp_add_line(file, 255,255, 511, 255, 1, a);
    bmp_add_line(file, 255,255, 511, 127, 1, a);
    bmp_add_line(file, 255,255, 511, 0, 1, a);

    bmp_add_line(file, 555,555, 560, 1000, 1, a);
    bmp_add_dot(file, 555, 555, 3, c);
    bmp_add_dot(file, 565, 1000, 3, c);

    /*bmp_add_line(file, 100,100 , 10, 200,1, a);
    a.r = 255;
    a.g = 0;
    a.b = 0;
    bmp_add_dot(file, 2, 255, 3, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(file, 511, 382, 3, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(file, 100, 511, 3, a);
    */
    bmp_generate_bmp(file,"test.bmp");

    return 0;
}

