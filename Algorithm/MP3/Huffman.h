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

struct Node {
	Node(char character, __int64_t count) {
		this->character = character;
		this->count = count;
		this->left = NULL;
		this->right = NULL;
	}

	char character;
	__int64_t count;
	Node *left;
	Node *right;
};

class Huffman {
	public:
		Huffman();
		~Huffman();

		void compress(std::string input);
		// void decompress(std::string input);

	private:
		void encode();
		// void decode();
		void traverse(Node *node, std::string code);
		void deleteTree(Node *node);

	protected:
		struct Node *_root;
		std::string _codes[256];
		std::map<char, __int64_t> _frequency;
		std::ifstream _infile;
		std::ofstream _outfile;
};

#endif