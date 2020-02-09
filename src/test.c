#include "lwlw.h"


lwlw_pixel write_callback(lwlw_pixel pixel,  int row, int col, int length) {
    printf("(%d,%d): ", row, col);
    for(int i = 0; i < length; i++) {
        printf("%03d ", pixel[i]);

        //changing pixel values
        //pixel[i] = i;
    }
    printf("\n");
    return pixel;
}

int main(int argc, char* argv[]) {

    lwlw_image image = lwlw_open_image(argv[1], LWLW_RGB);

    lwlw_override_image(image, write_callback);
    //lwlw_override_image(image, write_callback);

    lwlw_close_image(image);

    return 0;

}
