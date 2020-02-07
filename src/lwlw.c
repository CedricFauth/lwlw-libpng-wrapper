#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "lwlw.h"

static void error(const char* location, const char* message){

    fprintf(stderr, "\033[31mlwlw error\033[0m: %s: %s\n", location, message);

}

lwlw_image lwlw_open_image(const char* filename){

    FILE *fp = fopen(filename, "rb");
    
    if(!fp){
        error("read_image", "could not open file");
        return NULL;
    }

    u_int8_t header[8] = {0};
    fread(header, 1, 8, fp);
    if(png_sig_cmp(header, 0, 8)){
        error("read_image", "file is no png image");
        fclose(fp);
        return NULL;
    }

    lwlw_image image = calloc(1, sizeof(struct lwlw_image_t));
    if(!image){
        error("read_image", "could not create image structure");
        fclose(fp);
        return NULL;
    }

    image->png_struct_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL ,NULL);
    if(!image->png_struct_p){
        free(image);
        fclose(fp);
        error("read_image", "could not create png structure");
        return NULL;
    }

    image->png_info_p = png_create_info_struct(image->png_struct_p);
    if(!image->png_info_p){
        png_destroy_read_struct(&(image->png_struct_p), NULL, NULL);
        free(image);
        fclose(fp);
        error("read_image", "could not create info structure");
        return NULL;
    }

    png_set_sig_bytes(image->png_struct_p, 8);

    png_init_io(image->png_struct_p, fp);
    png_set_keep_unknown_chunks(image->png_struct_p, 1, NULL, 0);
    
    png_read_info(image->png_struct_p, image->png_info_p);

    image->width = png_get_image_width(image->png_struct_p, image->png_info_p);
    image->height = png_get_image_height(image->png_struct_p, image->png_info_p);
    png_byte color = png_get_color_type(image->png_struct_p, image->png_info_p);
    png_byte depth = png_get_bit_depth(image->png_struct_p, image->png_info_p);

    if(depth == 16)
        png_set_strip_16(image->png_struct_p);

    if(color == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(image->png_struct_p);

    if(color == PNG_COLOR_TYPE_GRAY && depth < 8)
        png_set_expand_gray_1_2_4_to_8(image->png_struct_p);

    if(png_get_valid(image->png_struct_p, image->png_info_p, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(image->png_struct_p);

    if(color == PNG_COLOR_TYPE_RGB ||
        color == PNG_COLOR_TYPE_GRAY ||
        color == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(image->png_struct_p, 0xFF, PNG_FILLER_AFTER);

    if(color == PNG_COLOR_TYPE_GRAY ||
        color == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(image->png_struct_p);
    
    png_read_update_info(image->png_struct_p, image->png_info_p);
    
    image->row_pointers = calloc(image->height, sizeof(png_bytep));

    image->row_length = png_get_rowbytes(image->png_struct_p, image->png_info_p);

    for (int row = 0; row < image->height; row++)
        image->row_pointers[row] = png_malloc(image->png_struct_p, image->row_length);

    png_read_image(image->png_struct_p, image->row_pointers);

    png_read_end(image->png_struct_p, NULL);

    png_destroy_read_struct(&(image->png_struct_p), &(image->png_info_p), NULL);
    fclose(fp);

    return image;

}

void lwlw_close_image(lwlw_image image){

    for (int i = 0; i < image->height; i++){
        free(image->row_pointers[i]);
    }
    
    free(image->row_pointers);
    free(image);

}

void lwlw_read_image(lwlw_image image, 
    void (*pixel_op)(png_bytep rgb_pixel, int row, int col, int length)){
    
    /*for (int i = 0; i < image->row_height; i++){
        png_bytep row = image->row_pointers[i];
        for(int j = 0; j < image->row_length / image->pixel_size; j++){
            pixel_op(&(row[j*image->pixel_size]), j, i, image->pixel_size);
        }
    }*/

    u_int8_t pixel_size = image->row_length / image->width;
    for (int i = 0; i < image->height; i++){
        for(int j = 0; j < image->width; j++){
            pixel_op(&(image->row_pointers[i][j*pixel_size]), i, j, pixel_size);
        }
    }

    /*for (int i = 0; i < image->height; i++){
        for(int j = 0; j < image->row_length; j++){
            printf("%d ", image->row_pointers[i][j]);
        }
    }*/

}
