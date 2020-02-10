# Documentation

## OPEN
|return type | function    |
|------------|-------------|
| lwlw_image | lwlw_open_image(const char* filename, int mode)|

&nbsp;

## READ / MODIFY
|return type | function    |
|------------|-------------|
| void       | lwlw_read_image( lwlw_image img, void (*pixel_op)(png_bytep rgb_pixel, int row, int col, int length) )|
| u_int_32_t  | lwlw_get_width( lwlw_image img ) |
| u_int_32_t  | lwlw_get_height( lwlw_image img )|
| void       | lwlw_override_image( lwlw_image img, lwlw_pixel (*pixel_op)(lwlw_pixel rgb_pixel, int row, int col, int length) )|
| lwlw_pixel | lwlw_get_pixel( lwlw_image, u_int32_t row, u_int32_t col )|

&nbsp;

## WRITE BACK / SAVE
|return type | function    |
|------------|-------------|
| int        | lwlw_write_image( lwlw_image img, char *filename )|

&nbsp;

## CLOSE
|return type | function    |
|------------|-------------|
| void       | lwlw_close_image( lwlw_image image )|

&nbsp;
