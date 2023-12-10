#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <bitset>

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
		void decode(size_t fileSize);
		void traverse(std::string code, Node *node);
		void makeHeader(Node *node, std::string &header);
		void stringToBinary(std::string &str);
		std::string binaryToString(unsigned char byte, int size);
		void deleteTree(Node *node);

	protected:
		Node *_root;
		std::string _body;
		std::unordered_map<char, std::string> _codebook;
		std::ifstream _infile;
		std::ofstream _outfile;
};

#endif