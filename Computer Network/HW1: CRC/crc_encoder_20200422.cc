#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av) {

    if (ac != 5) {
        printf("usage: ./crc_encoder input_file _output_file generator size_dataword\n");
        exit 1;
    }
    FILE *fp_input;
    if (!(fp_input = fopen(av[1], "r"))) {
        printf("input file open error.\n");
        exit 1;
    }
    FILE *fp_output;
    if (!(fp_output = fopen(av[2], "w"))) {
        printf("output file open error.\n");
        exit 1;
    }
    int size_dataword = av[4];
    if (!(size_dataword == 4 || size_dataword == 8)) {
        printf("dataword size must be 4 or 8.\n");
        exit 1;
    }
    int size_generator = strlen(av[3]);
    int size_codeword = size_dataword + size_generator - 1;


    char *string;
    fgets(string, MAX_STRING_SIZE, fp_str);

    fclose(fp_input);
    fclose(fp_output);
    return 0;
}