#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void modulo2division(char* dividend, char* generator, int generator_size, char* remainder) {

	char tmp[5];
	int codeword_size = -1, i, j;
	
	strcpy(tmp, dividend);
	while (++codeword_size < 4 - generator_size + 1) {
		// if the leftmost bit is 0, skip this iteration
		if (tmp[codeword_size] == '0')
			continue;
		
		// perform XOR operation
		for (i = 0, j = codeword_size; i < generator_size; i++, j++) {
			if (tmp[j] == generator[i])
				tmp[j] = '0';
			else
				tmp[j] = '1';
		}
	}
	printf("t before r: %s\n", tmp); //TODO
	strncpy(remainder, &tmp[codeword_size], generator_size - 1);
}

static void encode(FILE *fp_input, FILE *fp_output, char *generator, unsigned int dataword_size) {

	int generator_size = strlen(generator);
	char *chunk = (char *)malloc(sizeof(char));
	char *codeword = (char *)malloc(sizeof(char) * (dataword_size + generator_size));
	char *remainder = (char *)malloc(sizeof(char) * (generator_size - 1));
	if (!chunk || !codeword || !remainder) { printf("Error: malloc failed\n"); exit(1); }
	
	// padding bits to make the codeword size a multiple of 8
	int padding_bits = (8 - (generator_size - 1) % 8) % 8;
	for (int i = 0; i < padding_bits; i++)	// write padding bits to output file
		fputc('0', fp_output);
	
	// read input file in datawords and encode each one
	while (fscanf(fp_input, "%s", chunk) != EOF) {
		int len = strlen(chunk);
		for (int i = 0; i < len; i++) {
			// get remainder
			modulo2division(chunk, generator, generator_size, remainder);
			printf("remainder: %s\n", remainder); //TODO
			// concatenate dataword and remainder to create codeword
			strcpy(codeword, chunk);
			strcat(codeword, remainder);

			// write codeword to output file
			for (int i = 0; (i < dataword_size + generator_size + padding_bits - 1); i++) {
				printf("%c", codeword[i]); //TODO
				fputc(codeword[i], fp_output);
			}
			printf("\n\n"); //TODO
		}
	}
	free(chunk);
	free(codeword);
	free(remainder);
}

int main(int ac, char *av[]) {

	if (ac != 5) {
		printf("usage: ./crc_encoder input_file _output_file generator dataword_size\n"); exit(1);
	}

	FILE *fp_input = fopen(av[1], "rb");
	if (!fp_input) { printf("input file open error.\n"); exit(1); }
	FILE *fp_output = fopen(av[2], "wb");
	if (!fp_output) { printf("output file open error.\n"); exit(1); }

	// parse generator polynomial from binary string
	// unsigned int generator = strtol(av[3], NULL, 2);
	char *generator = av[3];
	unsigned int dataword_size = atoi(av[4]);
	if (!(dataword_size == 4 || dataword_size == 8)) {
		printf("dataword size must be 4 or 8.\n"); exit(1);
	}
	encode(fp_input, fp_output, generator, dataword_size);
	fclose(fp_input);
	fclose(fp_output);
	return 0;
}
