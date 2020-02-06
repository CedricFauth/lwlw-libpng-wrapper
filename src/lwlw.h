#ifndef LWLW_HEADER
#define LWLW_HEADER

#include "png.h"

struct lwlw_image_t {

    png_structp png_struct_p;
    png_infop png_info_p;

};

typedef struct lwlw_image_t *lwlw_image;

lwlw_image read_image(char* filename);

#endif
