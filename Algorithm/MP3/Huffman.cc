#include "Huffman.h"

Huffman::Huffman() : _root(NULL) {
}

Huffman::~Huffman() {
	if (_root != NULL) {
		deleteTree(_root);
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

Node::Node(char symbol, __int64_t frequency) : \
	left(NULL), right(NULL), freq(frequency), symbol(symbol) {
}

Node::Node() : \
	left(NULL), right(NULL), freq(0), symbol('\0') {
}

bool Node::operator()(const Node *lhs, const Node *rhs) const {
	return lhs->freq > rhs->freq;
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

	// Huffman encoding
	encode();

	// Write header (information for binary tree)
	writeHeader(_root);
	_outfile << std::endl;

	// Write body (encoded texts of infile)
	_infile.clear();
	_infile.seekg(0, std::ios::beg);
	char buf = '\0';
	while (_infile.get(buf)) {
		_outfile << _codebook[buf];
	}
}

void Huffman::encode() {
	// Read infile and count frequency
	std::map<char, __int64_t> frequency;
	char buf = '\0';
	while (_infile.get(buf)) {
		if (frequency.find(buf) == frequency.end()) {
			frequency[buf] = 1;
		} else {
			frequency[buf]++;
		}
	}

	// Create a priority queue sorted by frequency
	// Priority queue<data type, container, comparison>
	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	std::map<char, __int64_t>::iterator iter;
	for (iter = frequency.begin(); iter != frequency.end(); iter++) {
		pq.push(new Node(iter->first, iter->second));
	}

	// Turn the priority queue into a binary tree
	while (pq.size() > 1) {
		Node *leftChild = pq.top();
		pq.pop();
		Node *rightChild = pq.top();
		pq.pop();

		Node *parent = new Node('\0', leftChild->freq + rightChild->freq);
		parent->left = leftChild;
		parent->right = rightChild;
		pq.push(parent);
	}
	_root = pq.top();
	pq.pop();

	// Traverse the tree and assign codes
	for (int i = 0; i < 256; i++) {
		_codebook[i] = "";
	}
	std::string code = "";
	traverse(_root, code);
}

void Huffman::traverse(Node *node, std::string code) {
	if (node == NULL) {
		return;
	}
	if (node->left == NULL && node->right == NULL) {
		_codebook[(int)node->symbol] = code;
		return;
	}
	traverse(node->left, code + "0");
	traverse(node->right, code + "1");
}

void Huffman::writeHeader(Node* node) {
	if (node == NULL) {
		std::cout << "NULL" << std::endl;
		return;
	}
	if (node->left == NULL && node->right == NULL) {
		std::cout << node->symbol << std::endl;
		_outfile << node->symbol;
	} else {
		_outfile << '0'; // Indicator for internal node
		writeHeader(node->left);
		writeHeader(node->right);
	}
}

void Huffman::decompress(std::string input) {
	_infile.open(input, std::ios::binary);
	if (!_infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	_outfile.open(input + ".yy");
	if (!_outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}

	_root = readHeader();
	decode();

	_infile.close();
	_outfile.close();
}

Node* Huffman::readHeader() {
	// Read header and restructure the binary tree
	char bit;
	if (_infile.get(bit)) {
		if (bit == '0') {
			Node* internalNode = new Node('\0', 0);
			internalNode->left = readHeader();
			internalNode->right = readHeader();
			return internalNode;
		} else {
			// Leaf node
			return new Node(bit, 0);
		}
	}
	return NULL;
}

void Huffman::decode() {
	Node *node = _root;
	char bit = '\0';
	while (_infile.get(bit)) {
		if (bit == '0') {
			node = node->left;
		} else {
			node = node->right;
		}
		if (node->left == NULL && node->right == NULL) {
			_outfile << node->symbol;
			node = _root;
		}
	}
}
