#include "libs.h"
#include "types.h"

p_image get_image_data(char* path){
    //ERROR HANDELING OF ALL FUNCTIONS
    p_image img = malloc(sizeof(struct img_struct));
    img->data_stream = fopen(path, "rb");

    // Extract image dimensions from BMP header
    fseek(img->data_stream, 18, SEEK_SET);
    fread(&(img->width), sizeof(int), 1, img->data_stream);
    fread(&(img->height), sizeof(int), 1, img->data_stream);
    rewind(img->data_stream);

    fread(img->header, sizeof(BYTE), 54, img->data_stream);
    img->buffer = malloc(img->height * img->width * 3); // Tree bytes per pixel
    fread(img->buffer, 3, img->height* img->width, img->data_stream); // Read image data pixels

    return img;
}