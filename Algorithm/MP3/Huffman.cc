#include "Huffman.h"

Huffman::Huffman() {
	// root = NULL;
}

Huffman::~Huffman() {
	// deleteTree(root);
}

void Huffman::encode(std::string input) {
	std::ifstream infile(input);
	if (!infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	std::ofstream outfile(input + ".zz", std::ios::binary);
	if (!outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		infile.close();
		return;
	}
	
	// std::string line;
	// while (std::getline(infile, line)) {
	// 	for (int i = 0; i < line.length(); i++) {
	// 		std::cout << line[i] << std::endl;
	// 	}
	// }

	infile.close();
	outfile.close();
}

void Huffman::decode(std::string input) {
	std::ifstream infile(input, std::ios::binary);
	if (!infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	std::ofstream outfile(input + ".yy");
	if (!outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		infile.close();
		return;
	}

	infile.close();
	outfile.close();
}
