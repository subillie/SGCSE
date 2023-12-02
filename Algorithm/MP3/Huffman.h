#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
// #include <vector>
#include <map>
// #include <bitset>

struct Node {
	Node();
	Node(char symbol, __int64_t frequency);
	bool operator()(const Node *lhs, const Node *rhs) const;

	Node *left;
	Node *right;
	__int64_t freq;
	char symbol;
};

class Huffman {
	public:
		Huffman();
		~Huffman();

		void compress(std::string input);
		void decompress(std::string input);

	private:
		void encode();
		void decode();
		void traverse(Node *node, std::string code);
		void writeHeader(Node *node);
		Node *readHeader();
		void deleteTree(Node *node);

	protected:
		Node *_root;
		std::string _codebook[256];
		std::ifstream _infile;
		std::ofstream _outfile;
};

#endif