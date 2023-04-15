#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void modulo2division(char* codeword, char* generator, int codeword_size, int generator_size, char* remainder) {
	char tmp[codeword_size + 1];
	int i, j;
	
	strcpy(tmp, codeword);
	
	// perform modulo-2 division
	for (int k = 0; k < codeword_size - generator_size + 1; k++) {
		// if the leftmost bit is 0, skip this iteration
		if (tmp[k] == '0')
			continue;

		// perform XOR operation
		for (i = 0, j = k; i < generator_size; i++, j++) {
			if (tmp[j] == generator[i])
				tmp[j] = '0';
			else
				tmp[j] = '1';
		}
	}
	strncpy(remainder, &tmp[codeword_size - generator_size + 1], generator_size - 1);
}

static void decode(FILE *fp_output, FILE *fp_result, char *generator, unsigned int dataword_size) {

	int generator_size = strlen(generator);
	int codeword_size = dataword_size + generator_size - 1;
	char *codeword = (char *)malloc(sizeof(char) * (codeword_size + 1));
	char *remainder = (char *)malloc(sizeof(char) * (generator_size - 1));
	char *dataword = (char *)malloc(sizeof(char) * (dataword_size + 1));
	
	// read input file in codewords and decode each one
	while (fscanf(fp_output, "%s", codeword) != EOF) {
		int len = strlen(codeword);

		// get remainder
		modulo2division(codeword, generator, len, generator_size, remainder);
		
		// check if remainder is zero
		int is_valid = 1;
		for (int i = 0; i < generator_size - 1; i++) {
			if (remainder[i] != '0') {
				is_valid = 0;
				break;
			}
		}

		// if remainder is zero, extract dataword and write to output file
		if (is_valid) {
			strncpy(dataword, codeword, dataword_size);
			dataword[dataword_size] = '\0';
			fprintf(fp_result, "%s\n", dataword);
		}
	}
	free(codeword);
	free(remainder);
	free(dataword);
}

int main(int ac, char *av[]) {

	if (ac != 6) {
		printf("usage: ./crc_decoder input_file _output_file result_file generator dataword_size\n");
		exit(1);
	}

	FILE *fp_input = fopen(av[1], "rb");
	if (!fp_input) {
		printf("input file open error.\n");
		exit(1);
	}
	FILE *fp_output = fopen(av[2], "rb");
	if (!fp_output) {
		printf("output file open error.\n");
		exit(1);
	}
	FILE *fp_result = fopen(av[3], "wb");
	if (!fp_result) {
		printf("result file open error.\n");
		exit(1);
	}

	// parse generator polynomial from binary string
	// unsigned int generator = strtol(av[3], NULL, 2);
	char *generator = av[4];
	unsigned int dataword_size = atoi(av[5]);
	if (!(dataword_size == 4 || dataword_size == 8)) {
		printf("dataword size must be 4 or 8.\n");
		exit(1);
	}
	decode(fp_output, fp_result, generator, dataword_size);
	fclose(fp_input);
	fclose(fp_output);
	fclose(fp_result);
	return 0;
}
