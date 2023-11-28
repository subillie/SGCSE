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

void Huffman::compress(std::string input) {
	_infile.open(input);
	if (!_infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	_outfile.open(input + ".zz", std::ios::binary);
	if (!_outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}

	// Read infile and count frequency
	char buf = '\0';
	while (_infile.get(buf)) {
		if (_frequency.find(buf) == _frequency.end()) {
			_frequency[buf] = 0;
		} else {
			_frequency[buf]++;
		}
	}

	// Huffman encoding
	encode();

	// Write binary tree to outfile
	for (int i = 0; i < 256; i++) {
		_outfile << _codes[i] << " ";
	}

	// Write a newline to separate header from body
	_outfile << std::endl;

	// Write encoded texts to outfile
	buf = '\0';
	while (_infile.get(buf)) {
		_outfile << _codes[buf];
	}
}

Node::Node() : \
	left(NULL), right(NULL), count(0), character('\0') {
}

Node::Node(char c, __int64_t cnt) : \
	left(NULL), right(NULL), count(cnt), character(c) {
}

bool Node::operator()(const Node *lhs, const Node *rhs) const {
	return lhs->count > rhs->count;
}

void Huffman::encode() {
	// Create a priority queue sorted by frequency
	// Priority queue<data type, container, comparison>
	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	std::map<char, __int64_t>::iterator iter;
	for (iter = _frequency.begin(); iter != _frequency.end(); iter++) {
		pq.push(new Node(iter->first, iter->second));
	}

	// Turn the priority queue into a binary tree
	while (pq.size() > 1) {
		Node *leftChild = pq.top();
		pq.pop();
		Node *rightChild = pq.top();
		pq.pop();

		Node *parent = new Node('\0', leftChild->count + rightChild->count);
		parent->left = leftChild;
		parent->right = rightChild;
		pq.push(parent);
	}
	Node *root = pq.top();
	pq.pop();

	// Traverse the tree and assign codes
	std::string code = "";
	traverse(root, code);
}

void Huffman::traverse(Node *node, std::string code) {
	if (node->left == NULL && node->right == NULL) {
		_codes[node->character] = code;
		return;
	}
	traverse(node->left, code + "0");
	traverse(node->right, code + "1");
}

// void Huffman::decompress(std::string input) {
// 	_infile.open(input, std::ios::binary);
// 	if (!_infile.is_open()) {
// 		std::cerr << "File open error" << std::endl;
// 		return;
// 	}
// 	_outfile.open(input + ".yy");
// 	if (!_outfile.is_open()) {
// 		std::cerr << "File open error" << std::endl;
// 		return;
// 	}

// 	_infile.close();
// 	_outfile.close();
// }
