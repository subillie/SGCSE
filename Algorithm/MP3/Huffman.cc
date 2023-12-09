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
	std::string header = "";
	makeHeader(_root, header);
	std::bitset<8> bitset;
	bitset.reset();
	int count = 0;
	for (int i = 0; i < header.length(); i++) {
		bitset.set(count, header[i] == '1');
		count++;
		if (count == 8) {
			_outfile << bitset;
			bitset.reset();
			count = 0;
		}
	}
	if (count > 0) {
		_outfile << bitset;
	}
	_outfile << '\n';

	// Write body (encoded texts of infile)
	_infile.clear();
	_infile.seekg(0, std::ios::beg);
	char buf = '\0';
	std::string body = "";
	count = 0;
	while (_infile.get(buf)) {
		body += _codebook[(int)buf];
		while (body.length() >= 8) {
			bitset.set(count, body[0] == '1');
			count++;
			if (count == 8) {
				_outfile << bitset;
				bitset.reset();
				count = 0;
			}
			body = body.substr(1);
		}
	}
	if (count > 0) {
		_outfile << bitset;
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

void Huffman::makeHeader(Node* node, std::string& header) {
	if (node == NULL) {
		std::cout << "NULL" << std::endl;
		return;
	}
	if (node->left == NULL && node->right == NULL) {
		header += node->symbol;
	} else {
		header += '0';
		makeHeader(node->left, header);
		makeHeader(node->right, header);
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

	// Read header and restructure the binary tree
	char bit = '\0';
	bool isHeader = true;
	std::string header = "";
	while (_infile.get(bit) && isHeader) {
		std::bitset<8> bitset(bit);
		for (int i = 0; i < 8; i++) {
			if (bit == '\n') {
				isHeader = false;
				break;
			}
			header += bitset[i] ? '1' : '0';
		}
	}
	_infile.clear();
	_infile.seekg(0, std::ios::beg);
	_infile.ignore(1000, '\n');
	_root = readHeader(header, 0);
	decode();

	_infile.close();
	_outfile.close();
}

Node* Huffman::readHeader(std::string &header, int index) {
	if (header.length() == 0) {
		return NULL;
	}
	char bit = header[index];
	header = header.substr(1);
	if (bit == '0') {
		Node *internalNode = new Node('\0', 0);
		internalNode->left = readHeader(header, index + 1);
		internalNode->right = readHeader(header, index + 1);
		return internalNode;
	} else {
		// Leaf node
		char symbol = '\0';
		for (int i = 0; i < 8; i++) {
			symbol += (header[i] - '0') << (7 - i);
		}
		header = header.substr(8);
		return new Node(symbol, 0);
	}
}

void Huffman::decode() {
	char bit = '\0';
	std::bitset<8> bitset;
	std::string body = "";
	Node *node = _root;
	while (_infile.get(bit)) {
		std::bitset<8> bitset(bit);
		std::string str = bitset.to_string();
		for (int i = 0; i < str.length(); i++) {
			bit = str[i];
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
}
