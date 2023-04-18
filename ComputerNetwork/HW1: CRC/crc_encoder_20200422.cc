#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

string modulo2division(string dataword, string generator) {

	int generator_size = generator.length();
	string dividend = dataword + string(generator_size - 1, '0');
	string remainder = dividend.substr(0, generator_size);
	for (int i = generator_size; i < dividend.length(); i++) {
		if (remainder[0] == '1') {
			for (int j = 0; j < generator_size; j++) {
				if (remainder[j] == generator[j]) remainder[j] = '0';
				else remainder[j] = '1';
			}
		}
		remainder = remainder.substr(1) + dividend[i];
	}
	return remainder;
}

int main(int argc, char** argv) {

	// Check the number of arguments
	if (argc != 5) {
		// If wrong, let the user know the usage
		cerr << "usage: " << argv[0] << " input_file output_file generator dataword_size" << endl;
		return 1;
	}

	// Check if the params are valid, and initialize the variables
	ifstream input(argv[1], ios::binary);
	if (!input.is_open()) {
		cerr << "input file open error." << endl;
		return 1;
	}
	ofstream output(argv[2], ios::binary);
	if (!output.is_open()) {
		cerr << "output file open error." << endl;
		return 1;
	}
	string generator = argv[3];
	int dataword_size = atoi(argv[4]);
	if (dataword_size != 4 && dataword_size != 8) {
		cerr << "dataword size must be 4 or 8." << endl;
		return 1;
	}

	string dataword;
	while (input) {
		char c = input.get();
		if (input) {
			// Append the bitset on the dataword
			dataword += bitset<8>(c).to_string().substr(0, dataword_size);

			// If the dataword is full, do modulo2division and make the codeword
			if (dataword.length() != dataword_size) continue;
			string remainder = modulo2division(dataword, generator);

			// Make the codeword
			string codeword = dataword + remainder;
			int padding = 0;
			int codeword_size = codeword.length();
			if (codeword_size % 8 != 0)	// Add padding if the codeword size is not a multiple of 8
				padding = 8 - codeword_size % 8;
			codeword += string(padding, '0');

			// Write the codeword to the output file
			for (int i = 0; i < codeword_size + padding; i += 8) {
				char encoded_char = static_cast<char>(bitset<8>(codeword.substr(i, 8)).to_ulong());
				output.put(encoded_char);
			}
			dataword.clear();
		}
	}

	input.close();
	output.close();
	return 0;
}
