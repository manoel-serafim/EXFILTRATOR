#include "complexity.h"
#include "types.h"



p_secret get_secret_data(char* path){
    //ERROR HANDELING OF all functions
    p_secret ckrt = malloc(sizeof(struct secret_struct));
    ckrt->data_stream = fopen(path, "rb");
    // Determine the size of the data to embed
    fseek(ckrt->data_stream, 0, SEEK_END);
    ckrt->size = ftell(ckrt->data_stream);
    fseek(ckrt->data_stream, 0, SEEK_SET);
    ckrt->buffer = malloc(ckrt->size); 
    fread(ckrt->buffer, 1, ckrt->size, ckrt->data_stream);
    ckrt->buffer_index = 0;
    ckrt->bit_index = 0;
    return ckrt;
}



// Function to embed data into the image adaptively
void embed(char* img_path, char* file_path) {

    p_image image = get_image_data(img_path);
    p_secret data = get_secret_data(file_path);

    FILE *output = fopen("output.bmp", "wb");

    fwrite(image->header, sizeof(BYTE), 54, output); // Write BMP header
    
    double highest_var = max_var(image);
    // Process each pixel

    for (int y = 1; y < image->height - 1; y++) {
        for (int x = 1; x < image->width - 1; x++) {
            //will not use all bits for variance check , there will be canary bits
            // use only the Xth MSB
            BYTE window[3][3][3]; // 3x3 2D grid of pixels x3 for each pixel in each grid square

            // Fill the window with pixel data
            //dy dx are the offset from the pix
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int pix_pos = ((y + dy) * image->width + (x + dx)) * 3;
                    for (int channel = 0; channel < 3; channel++) {
                        window[dy + 1][dx + 1][channel] = image->buffer[pix_pos + channel];
                    }
                }
            }

            //should calculate the variance based on the first 5 MSB bits of each color chanel and never change them
            double complexity = calc_variance(window);
            int bitsToEmbed = (int)(round(complexity / (highest_var / 3)));
            //printf("%d, was embeded into the coordinate x:%d, y:%d", x,y );
            
            // Embed data based on complexity
            int pix_addr = (y * image->width + x) * 3;

            
            if (data->buffer_index < data->size) {
                for (int channel = 0; channel < 3; channel++) {
                    // Embed bitsToEmbed bits into each channel
                    for (int i = 0; i < bitsToEmbed; i++) {
                        // Embed the ith bit from data->buffer into the LSB of the color channel
                        image->buffer[pix_addr + channel] &= ~(1 << i); // zero i-th bit
                        image->buffer[pix_addr + channel] |= ((( *(data->buffer+data->buffer_index+data->bit_index)) >> i) & 1) << i; // 

                        // Increment the number of bits embedded
                        data->bit_index++;

                        // If 8 bits have been embedded, move to the next byte in data->buffer
                        if (data->bit_index > 7) {
                            data->buffer_index++;
                            data->bit_index = 0; // Reset the counter for the next byte
                        }
                    }
                }
            }else{
                fwrite(image->buffer, 3, image->height*image->width, output); // Write modified image data
                fclose(image->data_stream);
                free(image->buffer);
                free(image);

                fclose(data->data_stream);
                free(data->buffer);
                free(data);

                fclose(output);
                return;
            }
            
        }
    }
    
    fclose(image->data_stream);
    free(image->buffer);
    free(image);

    fclose(data->data_stream);
    free(data->buffer);
    free(data);

    fclose(output);
}


