#ifndef __TYPES_H_
#define __TYPES_H_

#include "libs.h"

typedef unsigned char BYTE;

struct img_struct{
    FILE* data_stream;
    int height;
    int width;
    BYTE* buffer;
    BYTE header[54];
    
};
typedef struct img_struct* p_image;

struct secret_struct{
    FILE* data_stream;
    int size;
    BYTE* buffer;
    int buffer_index;
    int bit_index;
};
typedef struct secret_struct* p_secret;

p_image get_image_data(const char *);

#endif