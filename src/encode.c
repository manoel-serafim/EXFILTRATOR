#include "complexity.h"
#include "types.h"



p_secret get_secret_data(const char* path) {
    // Allocate memory for the secret structure
    p_secret ckrt = malloc(sizeof(struct secret_struct));
    if (ckrt == NULL) {
        perror("malloc() error for secret struct");
        return NULL;
    }

    // Open the secret file
    ckrt->data_stream = fopen(path, "rb");
    if (ckrt->data_stream == NULL) {
        perror("fopen() error");
        free(ckrt);
        return NULL;
    }

    // Determine the size of the data to embed
    if (fseek(ckrt->data_stream, 0, SEEK_END) != 0) {
        perror("fseek() error");
        fclose(ckrt->data_stream);
        free(ckrt);
        return NULL;
    }

    ckrt->size = ftell(ckrt->data_stream);
    if (ckrt->size == -1) {
        perror("ftell() error");
        fclose(ckrt->data_stream);
        free(ckrt);
        return NULL;
    }

    if (fseek(ckrt->data_stream, 0, SEEK_SET) != 0) {
        perror("fseek() error");
        fclose(ckrt->data_stream);
        free(ckrt);
        return NULL;
    }

    // Allocate memory for the secret data buffer
    ckrt->buffer = malloc(ckrt->size);
    if (ckrt->buffer == NULL) {
        perror("malloc() error for buffer");
        fclose(ckrt->data_stream);
        free(ckrt);
        return NULL;
    }

    // Read the secret data
    if (fread(ckrt->buffer, 1, ckrt->size, ckrt->data_stream) != ckrt->size) {
        perror("fread() error");
        fclose(ckrt->data_stream);
        free(ckrt->buffer);
        free(ckrt);
        return NULL;
    }

    ckrt->buffer_index = 0;
    ckrt->bit_index = 0;

    fclose(ckrt->data_stream); // Close the file after reading
    return ckrt;
}



// Function to embed data into the image adaptively
void embed(const char * img_path, const char * file_path, const char * output_name) {

    p_image image = get_image_data(img_path);
    if (image == NULL) {
        fprintf(stderr, "Failed to load image data.\n");
        return;
    }

    p_secret data = get_secret_data(file_path);
    if (data == NULL) {
        fprintf(stderr, "Failed to load secret data.\n");
        free(image); // Free previously allocated image struct
        return;
    }

    FILE *output = fopen(output_name, "wb");
    if (output == NULL) {
        perror("Failed to open output file");
        free(image->buffer);
        free(image);
        free(data->buffer);
        free(data);
        return;
    }

    if (fwrite(image->header, sizeof(BYTE), 54, output) != 54) {
        perror("Failed to write BMP header to output file");
        fclose(output);
        free(image->buffer);
        free(image);
        free(data->buffer);
        free(data);
        return;
    }
    int data_embedded = 0;
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
                        image->buffer[pix_addr + channel] |= (((data->buffer[data->buffer_index]) >> data->bit_index) & 1) << i; // 

                        // Increment the number of bits embedded
                        data->bit_index++;
                        // If 8 bits have been embedded, move to the next byte in data->buffer
                        if (data->bit_index == 8) {
                            
                            data->buffer_index++;
                            data->bit_index = 0; // Reset the counter for the next byte
                        }
                    }
                    
                    
                }
            }else{
                data_embedded = 1;
                break;
            }
        }
        if (data_embedded) {
            break;
        }
    }

    if (!data_embedded) {
        fprintf(stderr, "[The image is too small to hide the whole file.]\n");
        fprintf(stderr, "Maximum that could be embedded was %d bits.\n", (8*data->buffer_index)+data->bit_index);
        fprintf(stderr, "Try adding an image with more complexity or separating the file into several images\n", (8*data->buffer_index)+data->bit_index);
    }

    // Write the modified image data to the output file
    if (fwrite(image->buffer, 3, image->height * image->width, output) != image->height * image->width) {
        perror("Failed to write modified image data to output file");
    }
    

    // Clean up resources
    fclose(output);
    free(image->buffer);
    free(image);
    free(data->buffer);
    free(data);
}


