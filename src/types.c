#include "libs.h"
#include "types.h"

p_image get_image_data(const char * path) {
    // Allocate memory for the image structure
    p_image img = malloc(sizeof(struct img_struct));
    if (img == NULL) {
        perror("malloc() error");
        return NULL;
    }

    // Open the image file
    img->data_stream = fopen(path, "rb");
    if (img->data_stream == NULL) {
        perror("fopen() error");
        free(img);
        return NULL;
    }

    // Extract image dimensions from BMP header
    if (fseek(img->data_stream, 18, SEEK_SET) != 0) {
        perror("fseek() error");
        fclose(img->data_stream);
        free(img);
        return NULL;
    }
    if (fread(&(img->width), sizeof(int), 1, img->data_stream) != 1 ||
        fread(&(img->height), sizeof(int), 1, img->data_stream) != 1) {
        perror("fread() error");
        fclose(img->data_stream);
        free(img);
        return NULL;
    }
    rewind(img->data_stream);

    // Read BMP header
    if (fread(img->header, sizeof(BYTE), 54, img->data_stream) != 54) {
        perror("fread() error on header");
        fclose(img->data_stream);
        free(img);
        return NULL;
    }

    // Allocate memory for the image buffer
    img->buffer = malloc(img->height * img->width * 3); // Three bytes per pixel
    if (img->buffer == NULL) {
        perror("malloc() error for buffer");
        fclose(img->data_stream);
        free(img);
        return NULL;
    }

    // Read image data pixels
    if (fread(img->buffer, 3, img->height * img->width, img->data_stream) != img->height * img->width) {
        perror("fread() error on image data");
        fclose(img->data_stream);
        free(img->buffer);
        free(img);
        return NULL;
    }
    fclose(img->data_stream);

    return img;
}
