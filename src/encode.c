#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef unsigned char BYTE;
//struct to define pixel and list

// Function to calculate the standard deviation of colors in a 3x3 window
double calc_variance(BYTE window[3][3][3]) { //pass it through in order to find entropy of file and stor in list
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

// Function to embed data into the image adaptively
void embedDataAdaptive(FILE *image, FILE *data, FILE *output, int width, int height) {
    BYTE header[54];
    fread(header, sizeof(BYTE), 54, image);  // Read BMP header
    fwrite(header, sizeof(BYTE), 54, output); // Write BMP header

    // Memory allocation for image data
    BYTE *imageBuffer = malloc(width * height * 3); // Tree bytes per pixel
    BYTE *dataBuffer = malloc(width * height);  
    fread(imageBuffer, 3, width * height, image); // Read image data

    size_t dataLength = fread(dataBuffer, 1, width * height, data); // Read data

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

            double complexity = calc_variance(window);
            int bitsToEmbed = complexity / 85 + 1; // Adaptive approach
            printf("%d, was embeded into the coordinate x:%d, y:%d", x,y );

            // Embed data based on complexity
            int pixelIndex = (y * width + x) * 3;
            int dataIndex = y * width + x;
            if (dataIndex < dataLength) {
                for (int i = 0; i < bitsToEmbed; i++) {
                    int channel = i % 3;
                    imageBuffer[pixelIndex + channel] = (imageBuffer[pixelIndex + channel] & ~1) | ((dataBuffer[dataIndex] >> i) & 1);
                }
            }
        }
    }

    fwrite(imageBuffer, 3, width * height, output); // Write modified image data

    free(imageBuffer); // Free memory
    free(dataBuffer);  // Free memory
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <image.bmp> <file_to_hide> <output_image.bmp>\n", argv[0]);
        return 1;
    }

    FILE *imageFile = fopen(argv[1], "rb");
    FILE *fileToHide = fopen(argv[2], "rb");
    FILE *outputFile = fopen(argv[3], "wb");

    if (!imageFile || !fileToHide || !outputFile) {
        perror("Error opening file");
        return 1;
    }

    // Extract image dimensions from BMP header
    fseek(imageFile, 18, SEEK_SET);
    int width, height;
    fread(&width, sizeof(int), 1, imageFile);
    fread(&height, sizeof(int), 1, imageFile);
    rewind(imageFile);

    embedDataAdaptive(imageFile, fileToHide, outputFile, width, height);

    fclose(imageFile);
    fclose(fileToHide);
    fclose(outputFile);
    return 0;
}
