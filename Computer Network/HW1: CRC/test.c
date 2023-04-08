#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATAWORD_SIZE 8

unsigned int generator; // global variable for the generator polynomial

// Function to calculate the CRC checksum of a dataword using the generator polynomial
unsigned int calculate_crc(unsigned int dataword, unsigned int dataword_size) {

	// shift dataword left to make room for dividend
	unsigned int dividend = dataword << (MAX_DATAWORD_SIZE - dataword_size);
	// shift generator left to make room for divisor
	unsigned int divisor = generator << (MAX_DATAWORD_SIZE - dataword_size);
	int i, j;
	for (i = 0; i < dataword_size; i++) { // perform modulo-2 division
		if (dividend & (1 << (MAX_DATAWORD_SIZE-1))) {
			dividend ^= divisor;
		}
		divisor >>= 1;
		dividend <<= 1;
	}
	return (dataword << (MAX_DATAWORD_SIZE - dataword_size)) | (dividend >> (MAX_DATAWORD_SIZE - dataword_size)); // combine dataword and remainder to form codeword
}

int main(int argc, char* argv[]) {
	if (argc != 5) { // check for correct number of arguments
		printf("usage: ./crc_encoder input_file output_file generator dataword_size\n");
		return -1;
	}
	char* input_file = argv[1];
	char* output_file = argv[2];
	generator = strtol(argv[3], NULL, 2); // parse generator polynomial from binary string
	unsigned int dataword_size = atoi(argv[4]);
	if (dataword_size != 4 && dataword_size != 8) { // check for valid dataword size
		printf("dataword size must be 4 or 8.\n");
		return -1;
	}
	FILE* fp_in = fopen(input_file, "rb"); // open input file for reading in binary mode
	if (!fp_in) { // check for file open error
		printf("input file open error.\n");
		return -1;
	}
	FILE* fp_out = fopen(output_file, "wb"); // open output file for writing in binary mode
	if (!fp_out) { // check for file open error
		printf("output file open error.\n");
		fclose(fp_in);
		return -1;
	}
	unsigned int dataword;
	while (fread(&dataword, dataword_size / 8, 1, fp_in) == 1) { // read datawords from input file
		unsigned int codeword = calculate_crc(dataword, dataword_size); // calculate CRC codeword
		fwrite(&codeword, (MAX_DATAWORD_SIZE + 2) / 8, 1, fp_out); // write codeword to output file
	}
	// pad output file with zeros to make it a multiple of 2 bytes
	unsigned int padding_size = ftell(fp_out) % 2;
	if (padding_size != 0) {
		unsigned int padding = 0;
		fwrite(&padding, padding_size, 1, fp_out);
	}
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}
