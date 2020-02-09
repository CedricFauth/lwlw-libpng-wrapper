#ifndef LWLW_HEADER
#define LWLW_HEADER

#include <sys/types.h>
#include "png.h"


#define LWLW_GRAY 0x01
#define LWLW_RGB 0x02
#define LWLW_SRGB 0x04


/*
    lwlw_image_t represents your image.
    lwlw_image is a pointer to that struct.

    lwlw_image provides private attributes
    for internal use only. Changing them will
    cause unexpected behavior.

    !do not access these attributes directly!
*/

typedef unsigned char* lwlw_pixel;

struct lwlw_image_t {

    //internals
    u_int32_t height;
    u_int32_t width;
    png_structp png_struct_p;
    png_infop png_info_p;
    png_bytepp row_pointers;
    size_t row_length;

};

/*
    use lwlw_image instead of struct lwlw_image_t
    (all functions of lwlw are using lwlw_image)
*/
typedef struct lwlw_image_t *lwlw_image;

/*
    lwlw_open_image opens an image if the given filename is correct
    it returns an lwlw_image pointer to the image struct
*/
lwlw_image lwlw_open_image(const char* filename, int mode);

/*
    call lwlw_close_image if you do not need it anymore.
    lwlw_close_image will free the image structure

    YOU NEED TO CLOSE EVERY IMAGE BEFORE YOU PROGRAM EXITS
*/
void lwlw_close_image(lwlw_image image);

/*
    lwlw_read_image lets you read the whole image pixel by pixel
*/
void lwlw_read_image(lwlw_image img,
                     void (*pixel_op)(png_bytep rgb_pixel, int row, int col, int length));

/*
    lwlw_override_image lets you modify every pixel of the image
    you can define a callback function that returns the new pixel value
*/
void lwlw_override_image(lwlw_image img,
                         lwlw_pixel (*pixel_op)(lwlw_pixel rgb_pixel, int row, int col, int length));


#endif
