
#include "types.h"
#include "complexity.h"

// Function to calculate the standard deviation of colors in a 3x3 window
double calc_variance(BYTE window[3][3][3]) {
    double sum = 0.0, mean, standardDeviation = 0.0;
    int i, j, chnl;
    int count = 0;

    // Calculating mean
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            for (chnl = 0; chnl < 3; ++chnl) {
                // Extracting 5 MSB bits and adding to sum
                sum += (window[i][j][chnl] >> 3) & 0x1F;//00011111
                count++;
            }
        }
    }
    mean = sum / count;

    // Calculating standard deviation
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            for (chnl = 0; chnl < 3; ++chnl) {
                // Extracting 5 MSB bits for each channel
                double pixelValue = (window[i][j][chnl] >> 3) & 0x1F;
                standardDeviation += pow(pixelValue - mean, 2);
            }
        }
    }

    return sqrt(standardDeviation / count);
}

double max_var(p_image img) {
    double highest_variance = 0.0;

    // Iterate through each 3x3 window in the image
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            BYTE window[3][3][3]; // 3x3 grid for the current window

            // Fill the window with pixel data
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int pixel_position = ((y + dy) * img->width + (x + dx)) * 3;
                    for (int channel = 0; channel < 3; channel++) {
                        window[dy + 1][dx + 1][channel] = img->buffer[pixel_position + channel];
                    }
                }
            }

            // Calculate variance for the current window
            double current_variance = calc_variance(window);
            if (current_variance > highest_variance) {
                highest_variance = current_variance;
            }
        }
    }

    return highest_variance;
}