#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <map>

class Huffman {
	public:
		Huffman();
		~Huffman();
		void encode(std::string input);
		void decode(std::string input);

	// private:
		// struct Node *newNode(char character, int frequency);
		// struct Node *mergeNodes(struct Node *left, struct Node *right);
		// void printCodes(struct Node *root, std::string str);
		// void encodeFile(std::string input, std::string output);
		// void decodeFile(std::string input, std::string output);
		// void printTree(struct Node *root, int indent);
		// void deleteTree(struct Node *root);

	// protected:
		// struct Node {
		// 	char _character;
		// 	int _frequency;
		// 	Node *_left;
		// 	Node *_right;
		// };

		// struct Node *_root;
		// std::string _codes[256];
};

#endif