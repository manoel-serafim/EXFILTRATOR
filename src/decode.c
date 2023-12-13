#include "types.h"
#include "complexity.h"




void disembed(const char * path, const char * output_name) {
    
    p_image image = get_image_data(path);
    if (image == NULL) {
        fprintf(stderr, "Failed to load image data.\n");
        return;
    }
    double highest_var = max_var(image);

    BYTE value = 0;
    int bit_count=0;

    FILE* output = fopen(output_name, "wb");
    if (output == NULL) {
        perror("Error opening output file");
        free(image->buffer);
        free(image);
        return;
    }


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

                    value |= (extractedBit << bit_count);//here is giving a segfault
                    // Increment the number of bits extracted
                    bit_count++;

                    // If 8 bits have been extracted, move to the next byte in data->buffer
                    if (bit_count == 8 ) {
                        if (fwrite(&value, 1, 1, output) != 1) {
                            perror("Error writing to output file");
                            fclose(output);
                            fclose(image->data_stream);
                            free(image->buffer);
                            free(image);
                            return;
                        }
                        // Write the extracted byte to the output file
                        value = 0;
                        bit_count = 0; // Reset the counter for the next byte

                        
                    }
                }
            }
        }
    }

    free(image->buffer);
    free(image);
    fclose(output); 
}
