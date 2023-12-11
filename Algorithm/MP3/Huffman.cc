#include "Huffman.h"

Huffman::Huffman() : _root(NULL), _body("") {
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
	_infile.open(input.c_str());
	if (!_infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	_outfile.open((input + ".zz").c_str(), std::ios::binary);
	if (!_outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}

	// Huffman encoding
	encode();
	size_t size = _codebook.size();
	unsigned char ucSize = size;
	_outfile << ucSize;

	// Write header
	std::map<char, std::string>::iterator iter;
	for (iter = _codebook.begin(); iter != _codebook.end(); iter++) {
		_outfile << (unsigned char)iter->first;
		unsigned char len = iter->second.length();
		_outfile << len;

		unsigned char byte = 0;
		for (size_t i = 0; i < len;) {
			for (int bit = 0; bit < 8; i++, bit++) {
				if (i >= len) {
					break;
				}
				byte = (byte << 1) | (iter->second[i] & 1);
			}
			_outfile << byte;
			byte = 0;
		}
	}

	// Write body
	unsigned char byte = 0;
	unsigned char remainder = 0;
	for (size_t i = 0; i < _body.length();) {
		for (int bit = 0; bit < 8; i++, bit++) {
			if (i >= _body.length()) {
				remainder = (unsigned char)bit;
				break;
			}
			byte = (byte << 1) | (_body[i] & 1);
		}
		_outfile << byte;
		byte = 0;
	}
	_outfile << remainder;
}

void Huffman::encode() {
	// Read infile and count frequency
	std::map<char, __int64_t> frequency;
	std::string text = "";
	char buf = '\0';
	while (_infile.get(buf)) {
		frequency[buf]++;
		text += buf;
	}
	size_t textLength = text.length();

	// Create a priority queue sorted by frequency
	// Priority queue<data type, container, comparison>
	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	std::map<char, __int64_t>::iterator iter;
	for (iter = frequency.begin(); iter != frequency.end(); iter++) {
		Node *node = new Node(iter->first, iter->second);
		pq.push(node);
	}

	// Turn the priority queue into a binary tree
	while (pq.size() > 1) {
		Node *left = pq.top();
		pq.pop();
		Node *right = pq.top();
		pq.pop();
		int freq = left->freq + right->freq;
		Node *parent = new Node('\0', freq);
		parent->left = left;
		parent->right = right;
		pq.push(parent);
	}
	_root = pq.top();
	pq.pop();

	traverse("", _root);
	for (size_t i = 0; i < textLength; i++) {
		char symbol = text[i];
		_body += _codebook[symbol];
	}
}

void Huffman::traverse(std::string code, Node *node) {
	if (node->left == NULL && node->right == NULL) {
		_codebook[node->symbol] = code;
		return;
	}
	code += '0';
	traverse(code, node->left);
	code[code.length() - 1] = '1';
	traverse(code, node->right);
}

void Huffman::decompress(std::string input) {
	_infile.open(input.c_str(), std::ios::binary);
	if (!_infile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}
	_outfile.open((input + ".yy").c_str());
	if (!_outfile.is_open()) {
		std::cerr << "File open error" << std::endl;
		return;
	}

	_infile.seekg(0, std::ios::end);
	size_t fileSize = _infile.tellg();
	_infile.seekg(0, std::ios::beg);
	decode(fileSize);

	_infile.close();
	_outfile.close();
}

void Huffman::decode(size_t fileSize) {
	// Read header
	size_t len = 1;
	size_t symbolNum = _infile.get();
	// Restore codebook
	for (size_t i = 0; i < symbolNum; i++) {
		char symbol = _infile.get();
		len++;
		int symbolLen = _infile.get();
		len++;

		// Read code
		std::string tmp = "";
		std::string code = "";
		while (symbolLen > 0) {
			unsigned char byte = _infile.get();
			len++;
			if (symbolLen > 8) {
				tmp = binaryToString(byte, 8);
			} else {
				tmp = binaryToString(byte, symbolLen);
			}
			code += tmp;
			symbolLen -= 8;
		}
		_codebook[symbol] = code;
	}

	// Read body
	fileSize -= len;
	std::string body = "";
	unsigned char byte = _infile.get();
	while (!_infile.fail()) {
		int size = 8;
		if (fileSize <= 2) {
			unsigned char remainder = _infile.get();
			size = (int)remainder;
		}
		fileSize--;
		body += binaryToString(byte, size);
		byte = _infile.get();
	}
	std::string code = "";
	for (size_t i = 0; i < body.length(); i++) {
		code += body[i];
		std::map<char, std::string>::iterator iter;
		for (iter = _codebook.begin(); iter != _codebook.end(); iter++) {
			if (code == iter->second) {
				_outfile << iter->first;
				code = "";
				break;
			}
		}
	}
}

std::string Huffman::binaryToString(unsigned char byte, int size) {
	std::string str = "";
	for (int i = 7; i >= 0; i--) {
		if (size <= i) {
			continue;
		}
		unsigned char bit = pow(2, i);
		str += (byte & bit) == bit ? '1' : '0';
	}
	return str;
}
