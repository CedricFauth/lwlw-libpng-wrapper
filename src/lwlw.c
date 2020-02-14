#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "lwlw.h"

static void error(const char* location, const char* message) {

    fprintf(stderr, "\033[31mlwlw error\033[0m: %s: %s\n", location, message);

}

lwlw_image lwlw_open_image(const char* filename, int mode) {

    // open file

    FILE *fp = fopen(filename, "rb");

    if(!fp) {
        error("lwlw_open_image", "could not open file");
        return NULL;
    }

    /*
        verify png header
        check if first 8 bytes of file are equal with png standard header
    */
    u_int8_t header[8] = {0};
    fread(header, 1, 8, fp);
    if(png_sig_cmp(header, 0, 8)) {
        error("lwlw_open_image", "file is no png image");
        fclose(fp);
        return NULL;
    }

    // allocate space for image structure
    lwlw_image image = calloc(1, sizeof(struct lwlw_image_t));
    if(!image) {
        error("lwlw_open_image", "could not create image structure");
        fclose(fp);
        return NULL;
    }

    // create png struct and save pointer in image struct
    image->png_struct_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,NULL);
    if(!image->png_struct_p) {
        free(image);
        fclose(fp);
        error("lwlw_open_image", "could not create png structure");
        return NULL;
    }

    // create png info struct and save pointer in image struct
    image->png_info_p = png_create_info_struct(image->png_struct_p);
    if(!image->png_info_p) {
        png_destroy_read_struct(&(image->png_struct_p), NULL, NULL);
        free(image);
        fclose(fp);
        error("lwlw_open_image", "could not create info structure");
        return NULL;
    }

    // tell libpng that signature was read
    png_set_sig_bytes(image->png_struct_p, 8);

    // preparing for reading image file
    png_init_io(image->png_struct_p, fp);
    png_set_keep_unknown_chunks(image->png_struct_p, 1, NULL, 0);

    // read info of png file first
    png_read_info(image->png_struct_p, image->png_info_p);

    // getting some important image attributes
    image->width = png_get_image_width(image->png_struct_p, image->png_info_p);
    image->height = png_get_image_height(image->png_struct_p, image->png_info_p);
    png_byte color = png_get_color_type(image->png_struct_p, image->png_info_p);
    png_byte depth = png_get_bit_depth(image->png_struct_p, image->png_info_p);

    // strip down to 8 bit per color-channel
    if(depth == 16)
        png_set_strip_16(image->png_struct_p);

    // set color type to rgb if necessary
    if(color == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(image->png_struct_p);

    // expand gray color channels to 8 bit each
    if(color == PNG_COLOR_TYPE_GRAY && depth < 8)
        png_set_expand_gray_1_2_4_to_8(image->png_struct_p);

    /*
        transformations depending on 'mode' attibute
    */
    if((mode == LWLW_RGBA || mode == LWLW_GRAYA) && png_get_valid(image->png_struct_p, image->png_info_p, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(image->png_struct_p);

    if(mode != LWLW_RGBA && mode != LWLW_GRAYA && color == PNG_COLOR_TYPE_RGB_ALPHA) {
        png_set_strip_alpha(image->png_struct_p);
    }

    if((mode == LWLW_RGBA || mode == LWLW_GRAYA) && (color == PNG_COLOR_TYPE_RGB ||
            color == PNG_COLOR_TYPE_GRAY ||
            color == PNG_COLOR_TYPE_PALETTE))
        png_set_filler(image->png_struct_p, 0xFF, PNG_FILLER_AFTER);

    if(mode == LWLW_GRAY || mode == LWLW_GRAYA) {
        png_set_rgb_to_gray(image->png_struct_p, PNG_ERROR_ACTION_NONE, 0.21, 0.72);
    }

    if(mode != LWLW_GRAY && mode != LWLW_GRAYA && (color == PNG_COLOR_TYPE_GRAY ||
            color == PNG_COLOR_TYPE_GRAY_ALPHA))
        png_set_gray_to_rgb(image->png_struct_p);
    /*
        done with transformations
    */

    // update struct
    png_read_update_info(image->png_struct_p, image->png_info_p);

    /*
        time to allocate space for pixels
        first: allocate rows
    */
    image->row_pointers = malloc(image->height * sizeof(u_int8_t*));

    // get number of bytes per row (width x channels)
    image->row_length = png_get_rowbytes(image->png_struct_p, image->png_info_p);

    // allocate space for each row
    for (int row = 0; row < image->height; row++)
        image->row_pointers[row] = png_malloc(image->png_struct_p, image->row_length);

    //finally read all pixels from image into row_bytes (u_int8_t**)
    png_read_image(image->png_struct_p, image->row_pointers);

    // end reading
    png_read_end(image->png_struct_p, NULL);

    // free all space that is unnecessary now
    png_destroy_read_struct(&(image->png_struct_p), &(image->png_info_p), NULL);
    // close file
    fclose(fp);

    return image;

}

void lwlw_close_image(lwlw_image image) {

    if(!image){
        error("lwlw_close_image", "image is NULL");
        return;
    }

    // free all pixels in rows
    for (int i = 0; i < image->height; i++) {
        free(image->row_pointers[i]);
    }

    //free row pointers
    free(image->row_pointers);
    // free image
    free(image);

}

void lwlw_override_image(lwlw_image image,
                         void (*pixel_op)(png_bytep rgb_pixel, int row, int col, int length)) {

    if(!image){
        error("lwlw_override_image", "image is NULL");
        return;
    }

    // determine how many channels a pixel contains
    u_int8_t pixel_size = image->row_length / image->width;
    // loop over each pixel
    for (int i = 0; i < image->height; i++) {
        for(int j = 0; j < image->width; j++) {
            // call modify/callback function with current pixel
            pixel_op(&(image->row_pointers[i][j*pixel_size]), i, j, pixel_size);
            
        }
    }

}


lwlw_pixel lwlw_get_pixel(lwlw_image image, u_int32_t row, u_int32_t col ){

    if(!image){
        error("lwlw_get_pixel", "image is NULL");
        return NULL;
    }
    int size = image->row_length / image->width;
    return (lwlw_pixel)&image->row_pointers[row][col*size];

}
