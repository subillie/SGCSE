#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void checkArguments(int ac, char **av, FILE *fp_input, FILE *fp_output);

int main(int ac, char **av) {

    FILE *fp_input, *fp_output;

    checkArguments(ac, av, fp_input, fp_output);
    int size_dataword = atoi(av[4]);
    int size_generator = strlen(av[3]);
    int size_codeword = size_dataword + size_generator - 1;
    int size_input = 0; while (!fgetc(fp_input)) size_input++;

    char ;


    // char *string;
    // fgets(string, MAX_STRING_SIZE, fp_str);


    fclose(fp_input);
    fclose(fp_output);
    return 0;
}

static void checkArguments(int ac, char **av, FILE *fp_input, FILE *fp_output) {

    if (ac != 5) {
        printf("usage: ./crc_encoder input_file _output_file generator size_dataword\n");
        exit(1);
    }
    if (!(fp_input = fopen(av[1], "r"))) {
        printf("input file open error.\n");
        exit(1);
    }
    if (!(fp_output = fopen(av[2], "w"))) {
        printf("output file open error.\n");
        exit(1);
    }
    if (!(av[4] == "4" || av[4] == "8")) {
        printf("dataword size must be 4 or 8.\n");
        exit(1);
    }
}