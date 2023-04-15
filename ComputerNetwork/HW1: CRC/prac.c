#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void modulo2division(char* dividend, char* divisor, int dividend_len, int divisor_len, char* remainder) {
	char tmp[256];
	int i, j, k;
	
	strcpy(tmp, dividend);
	
	// perform modulo-2 division
	for (i = 0; i < dividend_len - divisor_len + 1; i++) {
		// if the leftmost bit is 0, skip this iteration
		if (tmp[i] == '0')
			continue;
		
		// perform XOR operation
		for (j = 0, k = i; j < divisor_len; j++, k++) {
			if (tmp[k] == divisor[j])
				tmp[k] = '0';
			else
				tmp[k] = '1';
		}
	}
	
	// copy remainder into output string
	strncpy(remainder, &tmp[dividend_len-divisor_len+1], divisor_len-1);
}

// performs CRC encoding
void crc_encode(FILE* input_file, FILE* output_file, char* generator, int dataword_size) {
	char dataword[9];
	char codeword[16];
	char remainder[8];
	int i, len;
	
	// get generator length
	int generator_len = strlen(generator);
	
	// calculate the number of padding bits needed to make the codeword size a multiple of 8
	int padding_bits = (8 - (generator_len-1) % 8) % 8;
	
	// write padding bits to output file
	for (i = 0; i < padding_bits; i++) {
		fputc('0', output_file);
	}
	
	// read input file in datawords and encode each one
	while (fscanf(input_file, "%s", dataword) != EOF) {
		len = strlen(dataword);
		
		// perform modulo-2 division to get remainder
		modulo2division(dataword, generator, len, generator_len, remainder);
		
		// concatenate dataword and remainder to create codeword
		strcpy(codeword, dataword);
		strcat(codeword, remainder);
		
		// write codeword to output file
		for (i = 0; i < len+generator_len-1+padding_bits; i++) {
			fputc(codeword[i], output_file);
		}
	}
}

int main(int argc, char* argv[]) {
	FILE *input_file, *output_file;
	char generator[9];
	int dataword_size;
	
	// check for correct number of command line arguments
	if (argc != 4) {
		printf("Usage: %s input_file output_file generator\n", argv[0]);
		return 1;
	}
	
	// open input file
	input_file = fopen(argv[1], "r");
	if (input_file == NULL) {
		printf("Error: Cannot open input file %s\n", argv[1]);
		return 1;
	}
	
	// open output file
	output_file = fopen(argv[2], "w");
	if (output_file == NULL) {
		printf("Error: Cannot open output file %s\n", argv[2]);
		return 1;
	}
	
	// copy generator polynomial and get dataword size
	strcpy(generator, argv[3]);
	dataword_size = 8 - strlen(generator) + 1;
	
	// perform CRC encoding
	crc_encode(input_file, output_file, generator, dataword_size);
	
	// close files
	fclose(input_file);
	fclose(output_file);
	
	return 0;
}
