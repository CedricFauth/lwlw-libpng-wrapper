# Documentation

## Short Instructions

Here are some really short instructions on how to open, modify/read, save and close and image correctly with the lwlw library. To get further information please see the header file: ```lwlw.h```

Open:

1. To open an existing image use ```lwlw_open_image``` with a valid filename
2. ```lwlw_open_image``` will return a ```lwlw_image``` pointer to a struct. That struct contains all information about the image. You need to save this pointer in a variable to access the image data in the following steps.

Read & Change:

- You can use ```lwlw_override_image``` with ```your own modify-function as parameter``` to modify all pixels / loop over all pixels
- You may also use ```lwlw_get_pixel``` to get specific pixels by their position. You can change pixels by adressing the values ```lwlw_pixel``` points to
- ```lwlw_get_width``` and ```lwlw_get_height``` return the size of the image

Save:

- ```lwlw_save_image``` lets you save your image. The filename parameter may be NULL if you want to override the existing image

Close:

- If you don't need the image anymore you have to close it with ```lwlw_close_image```, otherwise it will stay in memory

&nbsp;

## Open
|return type | function    |
|------------|-------------|
| lwlw_image | lwlw_open_image(const char* filename, int mode)|

&nbsp;

## Read & modify
|return type | function    |
|------------|-------------|
| u_int_32_t | lwlw_get_width( lwlw_image img ) |
| u_int_32_t | lwlw_get_height( lwlw_image img )|
| void       | lwlw_override_image( lwlw_image img, void (*pixel_op)(lwlw_pixel rgb_pixel, int row, int col, int length) )|
| lwlw_pixel | lwlw_get_pixel( lwlw_image, u_int32_t row, u_int32_t col )|

&nbsp;

## Write back / save
|return type | function    |
|------------|-------------|
| int        | lwlw_save_image( lwlw_image img, char *filename )|

&nbsp;

## Close
|return type | function    |
|------------|-------------|
| void       | lwlw_close_image( lwlw_image image )|

&nbsp;
