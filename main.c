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

    BMPFILE *bmp_file = bmp_init_bmp(2,2,a); // width, height, background-color
    a.r = 0;
    a.g = 255;
    a.b = 255;
/*
    for(i =0; i<2000;++i){
        bmp_add_dot(bmp_file,0+i,0+i,2, a);
        for(j =0; j<2000;++j)
            bmp_add_dot(bmp_file,10,0+j,2, a);
    }
    file.bmp_add_line( 0,10 , 0, 200, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 0, 255, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 0, 511, 255, 255, 1, a); // bmp_pointer, x0, x1, y0, y1, strength(px), color
    bmp_add_line(file, 127,511,255, 255, 1, a);
    bmp_add_line(file, 255, 255, 255, 511, 1, a);
    bmp_add_line(file, 255, 255, 127+255, 511, 1, a);
    bmp_add_line(file, 255,255,511, 511, 1, a);
    bmp_add_line(file, 511, 127+255,255,255,  1, a);
    bmp_add_line(file, 255,255, 511, 127+255, 1, a);
    bmp_add_line(file, 255,255, 511, 255, 1, a);
    bmp_add_line(file, 255,255, 511, 127, 1, a);
    bmp_add_line(file, 255,255, 511, 0, 1, a);*/

    //bmp_add_line(file, 100,100 , 10, 200,1, a);
    a.r = 255;
    a.g = 0;
    a.b = 0;
    bmp_add_dot(bmp_file, 2, 255, 2, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(bmp_file, 511, 382, 1, a); // bmp_pointer, x, y, radius(px), color
    bmp_add_dot(bmp_file, 100, 511, 1, a);

    bmp_generate_bmp(bmp_file,"test.bmp");

    return 0;
}

