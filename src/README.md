# Documentation

## Functions

|return type | function    |
|------------|-------------|
| lwlw_image | lwlw_read_image( char *filename )|
| uint_32_t  | lwlw_get_width( lwlw_image img )|
| uint_32_t  | lwlw_get_height( lwlw_image img )|
| void       | lwlw_pixel_operation( lwlw_image img, uint_8_t (*pixel_op)(uint_8_t value, int pos_x, int pos_y) ) |
| int  | lwlw_write_image( lwlw_image img, char *filename )|

## Structures

### lwlw_image attributes:

| type        | attribute name  | description  |
|-------------|-----------------|-------------|
| png_structp | png_struct      | only internal use |
| png_infop   | png_info        | only internal use |
