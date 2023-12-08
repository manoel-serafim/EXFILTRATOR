#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef unsigned char BYTE;

double calculateStandardDeviation(BYTE window[3][3][3]) {
    double sum = 0.0, mean, standardDeviation = 0.0;
    int i, j, k;
    int count = 0;

    // Calculating mean
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            for (k = 0; k < 3; ++k) {
                sum += window[i][j][k];
                count++;
            }
        }
    }
    mean = sum / count;

    // Calculating standard deviation
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            for (k = 0; k < 3; ++k) {
                standardDeviation += pow(window[i][j][k] - mean, 2);
            }
        }
    }
    return sqrt(standardDeviation / count);
}

void extractDataFromImage(FILE *image, FILE *output, int width, int height) {
    BYTE header[54];
    fread(header, sizeof(BYTE), 54, image);  // Read BMP header

    // Memory allocation for image data
    BYTE *imageBuffer = malloc(width * height * 3);
    fread(imageBuffer, 3, width * height, image); // Read image data

    BYTE *dataBuffer = malloc(width * height); // Allocate memory for extracted data
    memset(dataBuffer, 0, width * height); // Initialize data buffer

    // Process each pixel
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            BYTE window[3][3][3]; // 3x3 window for each color channel

            // Fill the window with pixel data
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int pixelIndex = ((y + dy) * width + (x + dx)) * 3;
                    for (int channel = 0; channel < 3; channel++) {
                        window[dy + 1][dx + 1][channel] = imageBuffer[pixelIndex + channel];
                    }
                }
            }

            double complexity = calculateStandardDeviation(window);
            int bitsToExtract = complexity / 85 + 1; // Same approach as in embedding

            // Extract data based on complexity
            int pixelIndex = (y * width + x) * 3;
            int dataIndex = y * width + x;
            for (int i = 0; i < bitsToExtract; i++) {
                int channel = i % 3;
                dataBuffer[dataIndex] |= ((imageBuffer[pixelIndex + channel] & 1) << i);
            }
        }
    }

    fwrite(dataBuffer, 1, width * height, output); // Write extracted data

    free(imageBuffer); // Free memory
    free(dataBuffer);  // Free memory
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <encoded_image.bmp> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *encodedImageFile = fopen(argv[1], "rb");
    FILE *outputFile = fopen(argv[2], "wb");

    if (!encodedImageFile || !outputFile) {
        perror("Error opening file");
        return 1;
    }

    // Extract image dimensions from BMP header
    fseek(encodedImageFile, 18, SEEK_SET);
    int width, height;
    fread(&width, sizeof(int), 1, encodedImageFile);
    fread(&height, sizeof(int), 1, encodedImageFile);
    rewind(encodedImageFile);

    extractDataFromImage(encodedImageFile, outputFile, width, height);

    fclose(encodedImageFile);
    fclose(outputFile);
    return 0;
}
