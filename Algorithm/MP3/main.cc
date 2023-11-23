# include "Huffman.h"

int main (int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Usage: ./huffman [-c|-d] [input file]" << std::endl;
		return 1;
	}

	Huffman huffman;
	std::string flag = av[1];
	if (flag == "-c") {
		huffman.compress(av[2]);
	} else if (flag == "-d") {
		huffman.decompress(av[2]);
	} else {
		std::cerr << "Usage: ./huffman [-c|-d] [input file]" << std::endl;
		return 1;
	}

	return 0;
}