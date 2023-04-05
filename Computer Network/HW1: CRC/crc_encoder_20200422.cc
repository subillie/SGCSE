#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int ac, char **av) {

    FILE *fp_input, *fp_output;

    if (ac != 5) {
        printf("usage: ./crc_encoder input_file _output_file generator dataword_size\n");
        return 0;
    }
    if (!(fp_input = fopen(av[2], "r"))) {
        printf("input file open error.\n");
        return 0;
    }
    if (!(fp_output = fopen(av[3], "r"))) {
        printf("output file open error.\n");
        return 0;
    }
    if (!(av[5] == 4 || av[5] == 8)) {
        printf("dataword size must be 4 or 8.\n");
        return 0;
    }

    fgets(string, MAX_STRING_SIZE, fp_str);
    fgets(pattern, MAX_PATTERN_SIZE, fp_pat);
    fclose(fp_str);
    fclose(fp_pat);

    FILE *fp_res = fopen("result_kmp.txt", "w");
    getFailure(pattern);
    matchWithPattern(string, pattern, fp_res);
    fclose(fp_res);

    return 0;
}