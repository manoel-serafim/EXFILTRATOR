

#include "libs.h"
#include "encode.h"
#include "decode.h"


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <image.bmp> <file_to_hide>\n", argv[0]);
        return 1;
    }
    
    embed(argv[1], argv[2],"output.bmp");

    disembed("output.bmp", "output.bin");
    

    return 0;
}