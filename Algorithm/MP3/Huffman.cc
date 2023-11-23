#include "Huffman.h"

Huffman::Huffman() {
	// root = NULL;
}

Huffman::~Huffman() {
	// deleteTree(root);
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
		_infile.close();
		return;
	}

	// TODO: getline -> read
	std::string line;
	while (getline(_infile, line)) {
		encode(line);
	}

	_infile.close();
	_outfile.close();
}

void Huffman::encode(std::string line) {
	std::map<char, int> frequency;
	for (int i = 0; i < line.length(); i++) {
		frequency[line[i]]++;
	}

	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	for (auto iter = frequency.begin(); iter != frequency.end(); iter++) {
		pq.push(new Node(iter->first, iter->second));
	}

	while (pq.size() != 1) {
		Node *left = pq.top();
		pq.pop();
		Node *right = pq.top();
		pq.pop();

		Node *parent = new Node(left->_frequency + right->_frequency);
		parent->_left = left;
		parent->_right = right;
		pq.push(parent);
	}

	Node *root = pq.top();
	pq.pop();

	// std::string str;
	// printCodes(root, str);

	for (int i = 0; i < line.length(); i++) {
		_outfile << _codes[line[i]];
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
		_infile.close();
		return;
	}

	_infile.close();
	_outfile.close();
}
