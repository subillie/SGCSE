#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>

class Huffman {
	public:
		Huffman();
		~Huffman();

		void compress(std::string input);
		// void decompress(std::string input);

		// static const int CHUNK_SIZE = 1024;

	private:
		void encode(std::string input);

	protected:
		struct Node {
			char character;
			__int64_t frequency;
			Node *left;
			Node *right;
		};

		struct Node *_root;
		std::string _codes[256];
		std::map<char, __int64_t> _frequency;
		std::ifstream _infile;
		std::ofstream _outfile;
		// int _fdIf;
		// int _fdOf;
		// char *_buf;
};

#endif