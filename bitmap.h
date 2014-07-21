/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Victor Carlquist
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
*/
#ifndef BITMAPIMG_H
#define BITMAPIMG_H

#include <stdio.h>
#include <math.h>
#include <float.h>


enum {
    BMP_SCALE_NO,
    BMP_SCALE_FIT_XY,
    BMP_SCALE_FIT_X,
    BMP_SCALE_FIT_Y,
    BMP_X,
    BMP_Y
};

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}bmp_pixel;

typedef struct{
    float x0,x1,y0,y1;
    unsigned char strength;
    bmp_pixel px;
}bmp_line;

typedef struct{
    float x,y;
    int radius;
    bmp_pixel px;
}bmp_dot;

typedef struct{

    /*** BMP HEADER ***/
    unsigned char bmp_id_field[2];
    unsigned int width, height;
    unsigned char bmp_size_file[4];
    unsigned char bmp_reserved1[2];
    unsigned char bmp_reserved2[2];
    unsigned char bmp_offset_pixel[4];
    /*** END BMP HEADER ***/

    /*** DIB HEADER ***/
    unsigned char bmp_bytes_dib[4]; // 40 bytes
    unsigned char width_bmp[4]; //in pixels
    unsigned char height_bmp[4];
    unsigned char bmp_plane[2];
    unsigned char bmp_image_bits[2];
    unsigned char bmp_bi_rgb[4];
    unsigned char bmp_size_raw[4];
    unsigned char bmp_print_h_resolution[4];
    unsigned char bmp_print_v_resolution[4];
    unsigned char bmp_number_of_color_palette[4];
    unsigned char bmp_color_important[4];
    /*** END DIB HEADER ***/
    unsigned int bmp_index_raw;
    bmp_pixel *bmp_raw_data;
    unsigned int bmp_index_lines;
    bmp_line *bmp_raw_lines;
    unsigned int bmp_index_dots;
    bmp_dot *bmp_raw_dots;

    unsigned char scale_type;
    unsigned char calculated_scale;
    float dx,dy,minX,minY;


} BMPFILE;

//public
bmp_pixel bmp_create_pixel(unsigned char r, unsigned char g, unsigned char b)
{
    bmp_pixel color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

// private
void bmp_push_line(BMPFILE *bmp_file, bmp_line line)
{
    bmp_file->bmp_raw_lines[bmp_file->bmp_index_lines] = line;
    bmp_file->bmp_index_lines++;
}

//public
void bmp_add_line(BMPFILE *bmp_file,  float x0,  float y0,  float x1,  float y1, unsigned char strength, bmp_pixel color)
{
    bmp_line line;
    line.x0 = x0;
    line.y0 = y0;
    line.x1 = x1;
    line.y1 = y1;
    line.px = color;
    line.strength = strength;

    bmp_push_line(bmp_file, line);
}
// private
void bmp_push_dot(BMPFILE *bmp_file, bmp_dot dot)
{
    bmp_file->bmp_raw_dots[bmp_file->bmp_index_dots] = dot;
    bmp_file->bmp_index_dots++;
}

//public
void bmp_add_dot(BMPFILE *bmp_file, float x,  float y, unsigned char radius, bmp_pixel color)
{
    bmp_dot dot;
    dot.x = x;
    dot.y = y;
    dot.px = color;
    dot.radius = radius;

    bmp_push_dot(bmp_file, dot);
}
int bmp_calculate_scale(BMPFILE *bmp_file, float c, unsigned char type)
{
    if(!bmp_file->calculated_scale && bmp_file->scale_type != BMP_SCALE_NO){
        unsigned int i;
        float minX = FLT_MAX, minY = FLT_MAX;
        float maxX = FLT_MIN, maxY = FLT_MIN;
        for(i = 0;i < bmp_file->bmp_index_dots; ++i)
        {
            if(bmp_file->bmp_raw_dots[i].x > maxX)
                maxX = bmp_file->bmp_raw_dots[i].x;
            if(bmp_file->bmp_raw_dots[i].y > maxY)
                maxY = bmp_file->bmp_raw_dots[i].y;

            if(bmp_file->bmp_raw_dots[i].x < minX)
                minX = bmp_file->bmp_raw_dots[i].x;
            if(bmp_file->bmp_raw_dots[i].y < minY)
                minY = bmp_file->bmp_raw_dots[i].y;
        }
        for(i = 0;i < bmp_file->bmp_index_lines; ++i)
        {
            if(bmp_file->bmp_raw_lines[i].x0 > maxX)
                maxX = bmp_file->bmp_raw_lines[i].x0;
            if(bmp_file->bmp_raw_lines[i].y0 > maxY)
                maxY = bmp_file->bmp_raw_lines[i].y0;
            if(bmp_file->bmp_raw_lines[i].x1 > maxX)
                maxX = bmp_file->bmp_raw_lines[i].x1;
            if(bmp_file->bmp_raw_lines[i].y1 > maxY)
                maxY = bmp_file->bmp_raw_lines[i].y1;

            if(bmp_file->bmp_raw_lines[i].x0 < minX)
                minX = bmp_file->bmp_raw_lines[i].x0;
            if(bmp_file->bmp_raw_lines[i].y0 < minY)
                minY = bmp_file->bmp_raw_lines[i].y0;
            if(bmp_file->bmp_raw_lines[i].x1 < minX)
                minX = bmp_file->bmp_raw_lines[i].x1;
            if(bmp_file->bmp_raw_lines[i].y1 < minY)
                minY = bmp_file->bmp_raw_lines[i].y1;
        }
        if(minX >= 0)
            bmp_file->dx = (bmp_file->width-1)/fabs(fabs(maxX) - fabs(minX));
        else
            bmp_file->dx = (bmp_file->width-1)/fabs(fabs(maxX) + fabs(minX));
        if(minY >=0)
            bmp_file->dy = (bmp_file->height-1)/fabs(fabs(maxY) - fabs(minY));
        else
            bmp_file->dy = (bmp_file->height-1)/fabs(fabs(maxY) + fabs(minY));
        bmp_file->minX = minX;
        bmp_file->minY = minY;
        switch (bmp_file->scale_type) {
            case BMP_SCALE_FIT_X:
                bmp_file->dy = bmp_file->dx;
                break;
            case BMP_SCALE_FIT_Y:
                bmp_file->dx = bmp_file->dy;
                break;
        }
        bmp_file->calculated_scale = 1;
    }

    if(bmp_file->scale_type != BMP_SCALE_NO){
        if(type == BMP_X){
            if(bmp_file->minX >= 0)
                #ifdef __cplusplus
                    return int(bmp_file->dx*(c-bmp_file->minX));
                #else
                    return (int)(bmp_file->dx*(c-bmp_file->minX));
                #endif
            else
                #ifdef __cplusplus
                    return int(bmp_file->dx*(c+fabs(bmp_file->minX)));
                #else
                    return (int)(bmp_file->dx*(c+fabs(bmp_file->minX)));
                #endif
        }

        if(type == BMP_Y){
            if(bmp_file->minY >= 0)
                #ifdef __cplusplus
                    return int(bmp_file->dy*(c-bmp_file->minY));
                #else
                    return (int)(bmp_file->dy*(c-bmp_file->minY));
                #endif
            else
                #ifdef __cplusplus
                    return int(bmp_file->dy*(c+fabs(bmp_file->minY)));
                #else
                    return (int)(bmp_file->dy*(c+fabs(bmp_file->minY)));
                #endif
        }
    }
    return 0;
}
// private
void bmp_plot(BMPFILE *bmp_file, int x, int y, bmp_pixel color)
{
    if(x >= 0 && x < bmp_file->width && y < bmp_file->height && y >= 0)
        bmp_file->bmp_raw_data[(int)x + ((int)y)*bmp_file->width] = color;
}

//private
inline void bmp_swap(int *x, int *y)
{
    int a = *x;
    *x = *y;
    *y = a;
}
// private
void bmp_draw_lines(BMPFILE *bmp_file)
{
    int i,j;
    float x0,y0,x1,y1;
    float l,dx, dy;

    bmp_line *aux;
    for(i = 0; i < bmp_file->bmp_index_lines; ++i){
        aux = &bmp_file->bmp_raw_lines[i];
        if(bmp_file->scale_type != BMP_SCALE_NO){
            x0 = bmp_calculate_scale(bmp_file, aux->x0, BMP_X);
            y0 = bmp_calculate_scale(bmp_file, aux->y0, BMP_Y);
            x1 = bmp_calculate_scale(bmp_file, aux->x1, BMP_X);
            y1 = bmp_calculate_scale(bmp_file, aux->y1, BMP_Y);
        }else{
            x0 = aux->x0;
            y0 = aux->y0;
            x1 = aux->x1;
            y1 = aux->y1;
        }
        if( abs(x1-x0) >= abs(y1-y0))
            l = abs(x1-x0);
        else
            l = abs(y1-y0);
        dx = (float)(x1-x0) /(float) l;
        dy = (float)(y1-y0) /(float) l;
        x0 = x0 + 0.5 * ((dx < 0)? -1 : 1);
        y0 = y0 + 0.5 * ((dy < 0)? -1 : 1);
        for (j = 0; j < l; j++) {
            bmp_plot(bmp_file, floor(x0), floor(y0),aux->px);
            x0 += dx;
            y0 += dy;
        }
    }
}
// private
void bmp_draw_dots(BMPFILE *bmp_file)
{
    unsigned int i;
    int j,k,x,y;

    bmp_dot *aux;
    for(i = 0; i < bmp_file->bmp_index_dots; ++i){
        aux = &bmp_file->bmp_raw_dots[i];
        if(bmp_file->scale_type != BMP_SCALE_NO){
            x = bmp_calculate_scale(bmp_file, aux->x, BMP_X);
            y = bmp_calculate_scale(bmp_file, aux->y, BMP_Y);
        }else{
            x = aux->x;
            y = aux->y;
        }
        if(aux->radius == 1){
            bmp_plot(bmp_file, x, y, aux->px);
            continue;
        }
        for(k = -aux->radius-1; k <=aux->radius-1; ++k )
            for(j = -aux->radius-1; j <=aux->radius-1; ++j)
                bmp_plot(bmp_file,  x+j, y+k, aux->px);

    }
}

//public
void bmp_push(BMPFILE *bmp_file, bmp_pixel px)
{
    bmp_file->bmp_raw_data[bmp_file->bmp_index_raw] = px;
    bmp_file->bmp_index_raw++;
}

//public
bmp_pixel bmp_pop(BMPFILE *bmp_file)
{
    if(bmp_file->bmp_index_raw > 0)
        bmp_file->bmp_index_raw--;
    return bmp_file->bmp_raw_data[bmp_file->bmp_index_raw];
}

//public
unsigned int bmp_length(BMPFILE *bmp_file){return bmp_file->bmp_index_raw;}

//public
// w,h in pixel
BMPFILE *bmp_init_bmp(unsigned int w, unsigned int h, bmp_pixel bgcolor, unsigned char type_scale)
{
    BMPFILE *bmp_file = (BMPFILE*) malloc(sizeof(BMPFILE));
    bmp_file->scale_type = type_scale;
    bmp_file->calculated_scale = 0;

    bmp_file->width = w;
    bmp_file->height = h;

    bmp_file->bmp_raw_data = (bmp_pixel*) malloc(sizeof(bmp_pixel)*w*h);
    bmp_file->bmp_index_raw = 0;

    bmp_file->bmp_raw_lines = (bmp_line*) malloc(sizeof(bmp_line)*w*h);
    bmp_file->bmp_index_lines = 0;

    bmp_file->bmp_raw_dots = (bmp_dot*) malloc(sizeof(bmp_dot)*w*h);
    bmp_file->bmp_index_dots = 0;


    /*** BMP HEADER ***/
    bmp_file->bmp_id_field[0] = 0x42;
    bmp_file->bmp_id_field[1] = 0x4D;

    bmp_file->bmp_size_file[0] = 0x46;
    bmp_file->bmp_size_file[1] = 0x00;
    bmp_file->bmp_size_file[2] = 0x00;
    bmp_file->bmp_size_file[3] = 0x00;

    bmp_file->bmp_reserved1[0] = 0x00;
    bmp_file->bmp_reserved1[1] = 0x00;

    bmp_file->bmp_reserved2[0] = 0x00;
    bmp_file->bmp_reserved2[1] = 0x00;

    bmp_file->bmp_offset_pixel[0] = 0x36;
    bmp_file->bmp_offset_pixel[1] = 0x00;
    bmp_file->bmp_offset_pixel[2] = 0x00;
    bmp_file->bmp_offset_pixel[3] = 0x00;
    /*** END BMP HEADER ***/

    /*** DIB HEADER ***/
    bmp_file->bmp_bytes_dib[0] = 0x28;
    bmp_file->bmp_bytes_dib[1] = 0x00;
    bmp_file->bmp_bytes_dib[2] = 0x00;
    bmp_file->bmp_bytes_dib[3] = 0x00; // 40 bytes

    bmp_file->width_bmp[3] = (w >> 24) & 0xff;
    bmp_file->width_bmp[2] = (w >> 16) & 0xff;
    bmp_file->width_bmp[1] = (w >> 8) & 0xff;
    bmp_file->width_bmp[0] = w & 0xff;

    bmp_file->height_bmp[3] = (h >> 24) & 0xff;
    bmp_file->height_bmp[2] = (h >> 16) & 0xff;
    bmp_file->height_bmp[1] = (h >> 8) & 0xff;
    bmp_file->height_bmp[0] = h & 0xff;

    bmp_file->bmp_plane[0] = 0x01;
    bmp_file->bmp_plane[1] = 0x00;

    bmp_file->bmp_image_bits[0] = 0x18;
    bmp_file->bmp_image_bits[1] = 0x00;

    bmp_file->bmp_bi_rgb[0] = 0x00;
    bmp_file->bmp_bi_rgb[1] = 0x00;
    bmp_file->bmp_bi_rgb[2] = 0x00;
    bmp_file->bmp_bi_rgb[3] = 0x00;

    bmp_file->bmp_size_raw[0] = 0x10;
    bmp_file->bmp_size_raw[1] = 0x00;
    bmp_file->bmp_size_raw[2] = 0x00;
    bmp_file->bmp_size_raw[3] = 0x00;

    bmp_file->bmp_print_h_resolution[0] = 0x13;
    bmp_file->bmp_print_h_resolution[1] = 0x0b;
    bmp_file->bmp_print_h_resolution[2] = 0x00;
    bmp_file->bmp_print_h_resolution[3] = 0x00;

    bmp_file->bmp_print_v_resolution[0] = 0x13;
    bmp_file->bmp_print_v_resolution[1] = 0x0b;
    bmp_file->bmp_print_v_resolution[2] = 0x00;
    bmp_file->bmp_print_v_resolution[3] = 0x00;

    bmp_file->bmp_number_of_color_palette[0] = 0x00;
    bmp_file->bmp_number_of_color_palette[1] = 0x00;
    bmp_file->bmp_number_of_color_palette[2] = 0x00;
    bmp_file->bmp_number_of_color_palette[3] = 0x00;

    bmp_file->bmp_color_important[0] = 0x00;
    bmp_file->bmp_color_important[1] = 0x00;
    bmp_file->bmp_color_important[2] = 0x00;
    bmp_file->bmp_color_important[3] = 0x00;
    /*** END DIB HEADER ***/

    unsigned int i, total;
    total = h*w;
    for(i = 0; i < total; ++i)
        bmp_file->bmp_raw_data[i] = bgcolor;

    return bmp_file;

}

void bmp_generate_bmp(BMPFILE *bmp_file, char *file_name)
{
    FILE *fp;
    fp = fopen(file_name, "wb");
    if (fp == NULL)
        printf("\nBITMAPIMG: Error Open.");

    unsigned int size = bmp_file->width * bmp_file->height*3 +54;

    bmp_file->bmp_size_raw[3] = (size >> 24) & 0xff;
    bmp_file->bmp_size_raw[2] = (size >> 16) & 0xff;
    bmp_file->bmp_size_raw[1] = (size >> 8) & 0xff;
    bmp_file->bmp_size_raw[0] = size & 0xff;

    // BMP
    fwrite(bmp_file->bmp_id_field, sizeof(unsigned char)*2,1, fp);
    fwrite(bmp_file->bmp_size_file, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_reserved1, sizeof(unsigned char)*2,1, fp);
    fwrite(bmp_file->bmp_reserved2, sizeof(unsigned char)*2,1, fp);
    fwrite(bmp_file->bmp_offset_pixel, sizeof(unsigned char)*4,1, fp);
    //DIB
    fwrite(bmp_file->bmp_bytes_dib, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->width_bmp, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->height_bmp, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_plane, sizeof(unsigned char)*2,1, fp);
    fwrite(bmp_file->bmp_image_bits, sizeof(unsigned char)*2,1, fp);
    fwrite(bmp_file->bmp_bi_rgb, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_size_raw, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_print_h_resolution, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_print_v_resolution, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_number_of_color_palette, sizeof(unsigned char)*4,1, fp);
    fwrite(bmp_file->bmp_color_important, sizeof(unsigned char)*4,1, fp);

    unsigned int i,j;
    const unsigned char bmp_padding = 0x00;
    unsigned char time_padding = 0;
    unsigned int pixel_by_line = bmp_file->width_bmp[0]+(bmp_file->width_bmp[1]<<8)+(bmp_file->width_bmp[2]<<16)+(bmp_file->width_bmp[3]<<24);
    unsigned char needPadding = ((pixel_by_line%4) == 0) ? 0 : 1;
    if(needPadding)
        time_padding = (((pixel_by_line+1)%4) == 0) ? 1 : 2;
    unsigned int pixels = 0;
    //Raw pixel
    bmp_draw_lines(bmp_file);
    bmp_draw_dots(bmp_file);

    if(needPadding){
        for(i = 0; i < size ; ++i){
            fwrite(&bmp_file->bmp_raw_data[i], sizeof(bmp_pixel), 1, fp);
            if(needPadding){
                pixels++;
                if(pixels >= pixel_by_line){
                    pixels = 0;
                    for(j = 0; j < time_padding;++j)
                        fprintf(fp, "%c", bmp_padding);
                }
            }
        }
    }else
        fwrite(bmp_file->bmp_raw_data, sizeof(bmp_pixel), bmp_file->width*bmp_file->height-1, fp);
    fclose(fp);
    free(bmp_file->bmp_raw_data);
    free(bmp_file->bmp_raw_dots);
    free(bmp_file->bmp_raw_lines);
}
#endif // VBITMAPIMG_H
