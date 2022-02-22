BITMAPIMG
=========

Simple and **very fast** library for C or C++ to create dots and lines in a bitmap file.

# How to use it

Import the library in your code

```
#include "bitmap.h"
```

Create a color pixel:

```
    bmp_pixel red;
    red.r = 255;
    red.g = 0;
    red.b = 0;
    
    bmp_pixel white;
    red.r = 255;
    red.g = 255;
    red.b = 255;
    
```

Create BMPFILE object to draw the dots and lines

```
BMPFILE *file = bmp_init_bmp(1024, 1024, white, BMP_SCALE_NO); // width, height, background-color
```

Add a line

```
bmp_add_line(file, 0, 10 , 0, 200, 1, red); // bmp_pointer, x0, x1, y0, y1, strength(px), color
```

Add a dot

```
bmp_add_dot(file, 2, 255, 3, red); // bmp_pointer, x, y, radius(px), color
```

Save the bitmap image:
```
bmp_generate_bmp(file,"test.bmp");
```
