
#include "libs.h"
#include "encode.h"
#include "decode.h"

void print_help(const char* program_name) {
    printf("Usage: %s --mode <hide|show> [additional arguments]\n\n", program_name);
    printf("Modes:\n");
    printf("  hide: Hides a file within a BMP image.\n");
    printf("    Arguments: <input.bmp> <file_to_hide> <output.bmp>\n");
    printf("  show: Extracts a hidden file from a BMP image.\n");
    printf("    Arguments: <input.bmp> <output_file>\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_help(argv[0]);
        return 1;
    }

    const char* mode = NULL;
    int mode_index = -1;

    // Find the mode argument
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--mode") == 0 && i + 1 < argc) {
            mode = argv[i + 1];
            mode_index = i;
            break;
        }
    }

    if (mode == NULL) {
        fprintf(stderr, "Error: Missing '--mode' argument.\n");
        print_help(argv[0]);
        return 1;
    }

    if (strcmp(mode, "hide") == 0) {
        if (argc < mode_index + 4) {
            print_help(argv[0]);
            return 1;
        }
        const char *input_image = argv[mode_index + 2];
        const char *file_to_hide = argv[mode_index + 3];
        const char *output_image = argv[mode_index + 4];

        embed(input_image, file_to_hide, output_image);

    } else if (strcmp(mode, "show") == 0) {
        if (argc < mode_index + 3) {
            print_help(argv[0]);
            return 1;
        }
        const char *input_image = argv[mode_index + 2];
        const char *output_file = argv[mode_index + 3];

        disembed(input_image, output_file);

    } else {
        fprintf(stderr, "Error: Invalid mode '%s'. Use 'hide' or 'show'.\n", mode);
        print_help(argv[0]);
        return 1;
    }

    return 0;
}