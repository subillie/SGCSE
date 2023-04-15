#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DATAWORD_SIZE_MAX 8

// Perform modulo-2 division
int modulo2div(int dividend, unsigned char divisor) {
    int k = 3;
    int remainder = 0;

    for (int i = 31; i >= 0; i--) {
        remainder |= ((dividend >> k) & 1) << i;
        k--;
    }

    for (int i = 27; i >= 0; i--) {
        if ((remainder >> (i + 4)) & 1) {
            remainder ^= ((int)divisor << i);
        }
    }

    return remainder;
}

int main(int argc, char* argv[]) {

    if (argc != 5) {
        fprintf(stderr, "usage: %s input_file output_file generator dataword_size\n", argv[0]);
        return 1;
    }
    FILE *fp_input = fopen(argv[1], "rb");
    if (fp_input) {
        fprintf(stderr, "input file open error.\n");
        return 1;
    }
    FILE *fp_output = fopen(argv[2], "wb");
    if (!fp_output) {
        fprintf(stderr, "output file open error.\n");
        return 1;
    }
    unsigned int generator = (unsigned int)strtoul(argv[3], NULL, 2);
    int dataword_size = atoi(argv[4]);
    if (dataword_size != 4 && dataword_size != 8) {
        fprintf(stderr, "dataword size must be 4 or 8.\n");
        return 1;
    }

    int count = 0;
    int dataword = 0;
    unsigned char chunk;

    while (fread(&chunk, sizeof(unsigned char), 1, fp_input)) {
        // Append bits to dataword
        for (int i = 7; i >= 0; i--) {
            dataword |= ((chunk >> i) & 1) << count;
            count++;
            if (count == dataword_size) {
                // Perform modulo-2 division
                int remainder = modulo2div(dataword, generator);

                // Append remainder to dataword to form codeword
                dataword <<= 4;
                dataword |= remainder;

                count = 0;
                fwrite(&dataword, sizeof(int), 1, fp_output);
                dataword = 0;
            }
        }
    }

    // If there are any remaining bits in the dataword, pad with zeros and form the final codeword
    if (count > 0) {
        dataword <<= (dataword_size - count);
        int remainder = modulo2div(dataword, generator);
        dataword <<= 4;
        dataword |= remainder;
        fwrite(&dataword, sizeof(int), 1, fp_output);
    }

    fclose(fp_input);
    fclose(fp_output);

    return 0;
}
