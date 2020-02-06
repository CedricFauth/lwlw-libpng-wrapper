#include <stdlib.h>
#include "lwlw.h"

lwlw_image read_image(char* filename){

    lwlw_image image = malloc(sizeof(struct lwlw_image_t));

    image->png_struct_p = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL ,NULL);

    return image;

}
