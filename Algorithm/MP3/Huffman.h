#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>
// #include <unistd.h>
// #include <fcntl.h>

class Huffman {
	public:
		Huffman();
		~Huffman();

		void compress(std::string input);
		void decompress(std::string input);

	private:
		void encode(std::string input);

	protected:
		struct Node {
			char _character;
			int _frequency;
			Node *_left;
			Node *_right;
		};

		struct Node *_root;
		std::string _codes[256];
		std::map<char, int> _frequency;
		std::ifstream _infile;
		std::ofstream _outfile;
		// int _fdIf;
		// int _fdOf;
};

#endif