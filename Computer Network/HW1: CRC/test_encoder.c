#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void modulo2division(char* dividend, char* divisor, int dataword_size, int generator_size, char* remainder) {

	char tmp[256];
	int i, j, k;
	
	strcpy(tmp, dividend);
	
	// perform modulo-2 division
	for (i = 0; i < dataword_size - generator_size + 1; i++) {
		// if the leftmost bit is 0, skip this iteration
		if (tmp[i] == '0')
			continue;
		
		// perform XOR operation
		for (j = 0, k = i; j < generator_size; j++, k++) {
			if (tmp[k] == divisor[j])
				tmp[k] = '0';
			else
				tmp[k] = '1';
		}
	}
	
	// copy remainder into output string
	strncpy(remainder, &tmp[dataword_size - generator_size + 1], generator_size - 1);
}

// performs CRC encoding
void encode(FILE* fp_input, FILE* fp_output, char* generator, int dataword_size) {

	char *dataword = (char *)malloc(sizeof(char) * (dataword_size + 1));
	char codeword[16];
	char remainder[8];
	int i, len;

	// calculate padding bits to make the codeword size a multiple of 8
	int generator_size = strlen(generator);
	int padding_bits = (8 - (generator_size - 1) % 8) % 8;
	
	// write padding bits to output file
	for (i = 0; i < padding_bits; i++)
		fputc('0', fp_output);
	
	// read input file in datawords and encode each one
	while (fscanf(fp_input, "%s", dataword) != EOF) {
		len = strlen(dataword);
		
		// perform modulo-2 division to get remainder
		modulo2division(dataword, generator, len, generator_size, remainder);
		printf("remainder: %s\n", remainder); //TODO
		
		// concatenate dataword and remainder to create codeword
		strcpy(codeword, dataword);
		strcat(codeword, remainder);
		
		// write codeword to output file
		for (i = 0; i < len + generator_size - 1 + padding_bits; i++) {
			printf("%c", codeword[i]); //TODO
			fputc(codeword[i], fp_output);
		}
		printf("\n\n"); //TODO
	}
	free(dataword);
}

int main(int ac, char *av[]) {

	if (ac != 5) {
		printf("usage: ./crc_encoder input_file _output_file generator dataword_size\n");
		exit(1);
	}
	FILE *fp_input = fopen(av[1], "r");
	if (!fp_input) { printf("input file open error.\n"); exit(1); }
	FILE *fp_output = fopen(av[2], "wb");
	if (!fp_output) { printf("output file open error.\n"); exit(1); }
	
	char *generator = (char *)malloc(sizeof(char) * strlen(av[3]));
	strcpy(generator, av[3]);
	unsigned int dataword_size = atoi(av[4]);
	if (!(dataword_size == 4 || dataword_size == 8)) {
		printf("dataword size must be 4 or 8.\n"); exit(1);
	}
	
	encode(fp_input, fp_output, generator, dataword_size);
	fclose(fp_input);
	fclose(fp_output);
	free(generator);

	return 0;
}
