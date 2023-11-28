#include "Huffman.h"

Huffman::Huffman() : _root(NULL) {
	for (int i = 0; i < 256; i++) {
		_codes[i] = "";
	}
}

Huffman::~Huffman() {
	if (_root != NULL) {
		deleteTree(_root);
		_root = NULL;
	}
	if (_infile.is_open()) {
		_infile.close();
	}
	if (_outfile.is_open()) {
		_outfile.close();
	}
}

void Huffman::deleteTree(Node *node) {
	if (node == NULL) {
		return;
	}
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}
