#include "lwlw.h"

void read_callback(png_bytep rgb_pixel,  int row, int col, int length){
    printf("(%d,%d): ", row, col);
    for(int i = 0; i < length; i++)
        printf("%03d ", rgb_pixel[i]);
    printf("\n");
}

int main(int argc, char* argv[]){

    lwlw_image image = lwlw_open_image(argv[1]);

    lwlw_read_image(image, read_callback);

    lwlw_close_image(image);

    return 0;

}
