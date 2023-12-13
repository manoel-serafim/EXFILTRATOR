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


**Disclaimer**

The software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall the authors, copyright holders, or contributors be liable for any claim, damages, or other liability, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software. This software is not intended for use in any unlawful manner or context. Users are responsible for ensuring their use of the software complies with all applicable laws and regulations.


**Disclaimer**

The software is provided "as is", without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall the authors, copyright holders, or contributors be liable for any claim, damages, or other liability, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software. This software is not intended for use in any unlawful manner or context. Users are responsible for ensuring their use of the software complies with all applicable laws and regulations.
