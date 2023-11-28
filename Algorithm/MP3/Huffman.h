#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <map>

struct Node {
	Node();
	Node(char c, __int64_t cnt);
	bool operator()(const Node *lhs, const Node *rhs) const;

	Node *left;
	Node *right;
	__int64_t count;
	char character;
};

class Huffman {
	public:
		Huffman();
		~Huffman();

		void compress(std::string input);
		void decompress(std::string input);

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