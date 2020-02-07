#ifndef LWLW_HEADER
#define LWLW_HEADER

#include <sys/types.h>
#include "png.h"

struct lwlw_image_t {

    u_int32_t height;
    u_int32_t width;

    png_structp png_struct_p;
    png_infop png_info_p;
    png_bytepp row_pointers;
    size_t row_length;
};

typedef struct lwlw_image_t *lwlw_image;

lwlw_image lwlw_open_image(const char* filename);

void lwlw_close_image(lwlw_image image);

void lwlw_read_image(lwlw_image img, 
    void (*pixel_op)(png_bytep rgb_pixel, int row, int col, int length));

#endif
