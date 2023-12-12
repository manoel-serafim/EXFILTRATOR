#include "types.h"
#include "complexity.h"


//decode
p_secret gen_empty_secret(){
    //ERROR HANDELING OF all functions
    p_secret ckrt = malloc(sizeof(struct secret_struct));
    ckrt->data_stream = fopen("output.bin", "wb");
    ckrt->buffer= malloc(sizeof(BYTE)+1);
    ckrt->buffer_index = 0;
    ckrt->buffer[ckrt->buffer_index] = '0';
    ckrt->bit_index = 0;
    ckrt->size =0;
    return ckrt;
}



void disembed(char* path) {
    
    p_image image = get_image_data(path);
    p_secret data = gen_empty_secret();

    double highest_var = max_var(image);
    data->buffer_index = 0;
    data->bit_index = 0;

    for (int y = 1; y < image->height - 1; y++) {
        for (int x = 1; x < image->width - 1; x++) {
            BYTE window[3][3][3];

            // Fill the window with pixel data
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int pix_pos = ((y + dy) * image->width + (x + dx)) * 3;
                    for (int channel = 0; channel < 3; channel++) {
                        window[dy + 1][dx + 1][channel] = image->buffer[pix_pos + channel];
                    }
                }
            }

            // Calculate the complexity (variance)
            double complexity = calc_variance(window);
            int bitsToDisembed = (int)(round(complexity / (highest_var / 3)));

            int pix_addr = (y * image->width + x) * 3;

            for (int channel = 0; channel < 3; channel++) {
                for (int i = 0; i < bitsToDisembed; i++) {
                    // Extract the ith bit from the LSB of the color channel
                    int extractedBit = (image->buffer[pix_addr + channel] >> i) & 1;

                    *(data->buffer) |= (extractedBit << data->bit_index);//here is giving a segfault
                    // Increment the number of bits extracted
                    data->bit_index++;

                    // If 8 bits have been extracted, move to the next byte in data->buffer
                    if (data->bit_index == 8 ) {
                        printf("data:%s",data->buffer[data->buffer_index]);
                        fwrite(&data->buffer[data->buffer_index], 1, 1, data->data_stream);
                        // Write the extracted byte to the output file
                        data->bit_index = 0; // Reset the counter for the next byte

                        if (data->buffer_index >= data->size) {
                            fclose(data->data_stream);
                            return;
                        }
                    }
                }
            }
        }
    }
}
