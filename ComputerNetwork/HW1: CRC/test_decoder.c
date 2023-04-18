#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CODEWORD_SIZE 1024

int main(int argc, char *argv[]) {
    // Check argument count
    if (argc != 6) {
        printf("usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
        exit(1);
    }

    // Open input file
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("input file open error.\n");
        exit(1);
    }

    // Open output file
    FILE *output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        printf("output file open error.\n");
        exit(1);
    }

    // Open result file
    FILE *result_file = fopen(argv[3], "w");
    if (result_file == NULL) {
        printf("result file open error.\n");
        exit(1);
    }

    // Parse arguments
    unsigned int generator = strtol(argv[4], NULL, 2);
    int dataword_size = atoi(argv[5]);
    if (dataword_size != 4 && dataword_size != 8) {
        printf("dataword size must be 4 or 8.\n");
        exit(1);
    }

    // Read padding size
    int padding_size;
    fread(&padding_size, sizeof(char), 1, input_file);

    // Discard padding bits
    int discard_size = padding_size % 8;
    if (discard_size > 0) {
        fseek(input_file, 1, SEEK_CUR);
    }

    // Read and decode codewords
    int codeword_count = 0;
    int error_count = 0;
    unsigned char codeword[MAX_CODEWORD_SIZE];
    while (fread(codeword, sizeof(char), dataword_size + 2, input_file) == dataword_size + 2) {
        // Extract dataword and CRC bits
        unsigned int dataword = 0;
        memcpy(&dataword, codeword, dataword_size);
        unsigned int crc = 0;
        memcpy(&crc, codeword + dataword_size, 2);

        // Check CRC
        unsigned int remainder = dataword;
        for (int i = 0; i < 16; i++) {
            if (remainder & (1 << (dataword_size + i))) {
                remainder ^= generator << i;
            }
        }
        if (remainder != crc) {
            error_count++;
        }

        // Write dataword to output file
        fwrite(&dataword, sizeof(char), dataword_size, output_file);

        codeword_count++;
    }

    // Write result to result file
    fprintf(result_file, "%d %d\n", codeword_count, error_count);

    // Close files
    fclose(input_file);
    fclose(output_file);
    fclose(result_file);

    return 0;
}
