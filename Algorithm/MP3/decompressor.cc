#include "Huffman.h"

void Huffman::decompress(std::string input) {
	_infile.open(input, std::ios::binary);
	if (!_infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	_outfile.open(input + ".yy");
	if (!_outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}

	_infile.close();
	_outfile.close();
}