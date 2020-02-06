#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "lwlw.h"

static void error(const char* location, const char* message){

    fprintf(stderr, "\033[31mlwlw error\033[0m: %s: %s\n", location, message);

}


lwlw_image read_image(const char* filename){

    FILE *fp = fopen(filename, "rb");
    
    if(!fp){
        error("read_image", "could not open file");
        return NULL;
    }

    u_int8_t header[8] = {0};
    fread(header, 1, 8, fp);
    if(png_sig_cmp(header, 0, 8)){
        error("read_image", "file is no png image");
        return NULL;
    }

    lwlw_image image = calloc(1, sizeof(struct lwlw_image_t));
    if(!image){
        error("read_image", "could not create image structure");
        return NULL;
    }

    image->png_struct_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL ,NULL);
    if(!image->png_struct_p){
        free(image);
        error("read_image", "could not create png structure");
        return NULL;
    }

    image->png_info_p = png_create_info_struct(image->png_struct_p);
    if(!image->png_info_p){
        png_destroy_read_struct(&(image->png_struct_p), NULL, NULL);
        free(image);
        error("read_image", "could not create info structure");
        return NULL;
    }

    image->png_end_info_p = png_create_info_struct(image->png_struct_p);
    if(!image->png_end_info_p){
        png_destroy_read_struct(&(image->png_struct_p), &(image->png_info_p), NULL);
        free(image);
        error("read_image", "could not create info structure");
        return NULL;
    }



    return image;

}
