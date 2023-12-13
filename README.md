# EXFILTRATOR
## Advanced Data Extraction & Exfiltration

### Overview
Exfiltrator is a tool designed for secure and discreet data retrieval. Employing advanced adaptative steganography, it obscures data flows, significantly reducing detectability in compromised systems. This tool is ideal for sensitive operations where it is imperative that data movement remains hidden and protected.

### Key Features
- **Adaptive Steganography**: Utilizes adaptive steganography based on a comprehensive statistical analysis of the image. This approach allows for more effective concealment of data within areas of high visual complexity.
- **Native Library Usage**: To further obfuscate static analysis, Exfiltrator relies exclusively on native libraries.

### Usage as a Library
Exfiltrator is crafted to function seamlessly as a library, allowing easy integration into broader applications or systems. This design choice offers flexibility and adaptability for various use cases.

#### Embed Function

The `embed` function in Exfiltrator is designed to hide data within an image in a way that minimizes detectability. It employs a detailed analysis of the image using a window of 3x3 pixels to determine the complexity of different regions.

##### Embedding Process:

1. **Image Analysis for Complexity**:
    - The algorithm assesses each 3x3 pixel window of the image.
    - It performs bitwise operations to analyze the pixel values, determining the complexity of each window based on color variance and intensity differences.
    - Higher complexity regions are identified as better candidates for data embedding, as changes here are less perceptible.

2. **Adaptive Data Embedding**:
    - Based on the complexity analysis, the algorithm decides the number of bits that can be safely embedded in each window.
    - Bits of the secret data are embedded into the least significant bits (LSBs) of the pixels in the complex regions.
    - The embedding is done using bitwise operations, ensuring that the visual impact on the image is minimal.

![Embed Process](output/canary_bin.bmp)  *(Example Image Showing picture with Embedded Data)*

#### Disembed Function

The `disembed` function is the counterpart to `embed`, designed to extract the hidden data from an image. This function effectively reverses the embedding process.

##### Extraction Process:

1. **Image Processing for Data Extraction**:
    - The image is processed in the same 3x3 pixel windows as used during embedding.
    - The algorithm uses bitwise operations to extract the embedded bits from the LSBs of the pixels in each window.

2. **Data Retrieval**:
    - Extracted bits are compiled back into their original form, reconstructing the hidden data.
    - The function ensures the integrity and confidentiality of the extracted data, retrieving it exactly as it was embedded.

The `embed` and `disembed` functions demonstrate Exfiltrator's ability to utilize adaptive steganography based on statistical image analysis. This method is highly effective in concealing data within images, making it an invaluable tool for secure data transmission.

### Fine Tuning
Exfiltrator offers options for fine-tuning the steganography process, allowing users to balance between the amount of data to be hidden and the level of visual discretion.

![Embed Process](output/canary_diff.bmp)  *(Example Image Showing picture with Embedded Data inside of the picture and highlighted)*



---

There is also a main.c implementation of a program that uses exfiltrator and in the bin/exfilter, the compiled program that has the following usage

## Usage

Exfiltrate is equipped with two primary modes of operation: `hide` and `show`. These modes are used for embedding data into an image and extracting data from an image, respectively.

### Hide Mode

In `hide` mode, Exfiltrator embeds specified data into a given image. This mode is ideal for securely concealing data within an image file.

#### Command Format
```
exfilter --mode hide <input.bmp> <file_to_hide> <output.bmp>
```

- `input.bmp`: The source BMP image file in which the data will be hidden.
- `file_to_hide`: The file containing the data you wish to embed in the image.
- `output.bmp`: The output BMP image file with the hidden data embedded.

#### Example
```
exfilter --mode hide input.bmp secret.txt output.bmp
```

This command will take `secret.txt`, embed its contents into `input.bmp`, and produce `output.bmp` with the data securely hidden inside.

### Show Mode

In `show` mode, Exfiltrator extracts the hidden data from the specified image. This mode is used to retrieve the data previously embedded into an image file.

#### Command Format
```
exfilter --mode show <input.bmp> <output_file>
```

- `input.bmp`: The BMP image file from which the data is to be extracted.
- `output_file`: The file where the extracted data will be saved.

#### Example
```
exfilter --mode show output.bmp extracted_data.txt
```

This command will extract the hidden data from `output.bmp` and save it in `extracted_data.txt`.

---

# README: Exfiltrator vs Steghide Experiment - Detailed Analysis

## Overview

This experiment aimed to compare the performance and stealth characteristics of Exfiltrator with Steghide, a popular steganography tool that uses LSB. The focus was on how each tool embeds data into an image and the resulting detectability of modifications.

## Key Differences in Methodology

### Steghide:
- **LSB Technique**: Steghide primarily uses the Least Significant Bit (LSB) method for embedding data. This approach involves altering the LSBs of pixel data to store information.
- **Scattered Output**: The data embedding by Steghide tends to be scattered across the image. While this method can distribute data evenly, it can also make the changes more predictable and potentially more detectable, especially in images with large uniform color areas.
![Embed Process](experiment/dogdiffsteg.bmp)  *(Steghide difference from Original Image)*


### Exfiltrator:
- **Adaptive Steganography**: Exfiltrator utilizes an adaptive steganography technique that is based on the variance and complexity of the image. It analyzes different regions of the image to identify areas of high complexity where changes are less noticeable.
- **Variance-Related Data Embedding**: Unlike Steghide, Exfiltrator does not have a set number of bits for embedding across the entire image. Instead, the number of bits embedded in each region is related to the variance and the image's overall distribution. This means that more bits are embedded in areas of higher complexity, making changes less perceptible.
![Embed Process](experiment/dogdiffours.bmp)  *(Exfilter difference from Original Image)*

## Experiment Log and Observations

During the experiment, we observed:

- **Steghide Results**: In `dog_steg.bmp`, the modifications made by Steghide were more discernible when the image was compared to the original, as seen in `experiment/dogdiffsteg.bmp`. The changes were more scattered but predictable due to the LSB technique.

- **Exfiltrator Results**: The `dog_ours.bmp` image, modified by Exfiltrator, displayed subtler alterations. The comparison output in `experiment/dogdiffours.bmp` demonstrated that the changes were less noticeable, aligning with the areas of higher complexity in the image.

## Conclusion

The adaptive steganography approach of Exfiltrator, which bases data embedding on image variance, proves to be more effective for discreet embedding compared to the LSB method used by Steghide. Exfiltrator's technique ensures that the embedded data is less detectable, making it more suitable for scenarios where high-level concealment is required.

## Further Considerations

Additional research could explore the impact of Exfiltrator's method on various types of images, including those with different levels of complexity and color distribution. Experimenting with different types and sizes of data could also provide a more comprehensive understanding of the tool's versatility and effectiveness in diverse applications.


**Disclaimer**

The software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall the authors, copyright holders, or contributors be liable for any claim, damages, or other liability, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software. This software is not intended for use in any unlawful manner or context. Users are responsible for ensuring their use of the software complies with all applicable laws and regulations.
