#include "Huffman.h"

Huffman::Huffman() : _root(NULL) {
	for (int i = 0; i < 256; i++) {
		_codebook[i] = "";
	}
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

Node::Node(char character, __int64_t frequency) : \
	left(NULL), right(NULL), freq(frequency), symbol(character) {
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

	// Read infile and count frequency
	char buf = '\0';
	while (_infile.get(buf)) {
		if (_frequency.find(buf) == _frequency.end()) {
			_frequency[buf] = 1;
		} else {
			_frequency[buf]++;
		}
	}
	// std::cout << "++++++++ Frequency ++++++++" << std::endl;
	// for (auto iter = _frequency.begin(); iter != _frequency.end(); iter++) {
	// 	std::cout << iter->first << " " << iter->second << std::endl;
	// }
	// std::cout << "++++++++ Frequency ++++++++" << std::endl;

	// Huffman encoding
	encode();

	// Write header (encoded bits for each character)
	// std::cout << "++++++++ Codebook ++++++++" << std::endl;
	for (int i = 0; i < 256; i++) {
		std::cout << _codebook[i] << std::endl;
		if (_codebook[i] != "") {
			_outfile << static_cast<char>(i) << " " << _codebook[i] << " ";
		}
	}
	// Write newline to separate header from body
	_outfile << std::endl;
	// std::cout << "++++++++ Codebook ++++++++" << std::endl;

	// Write body (encoded texts of infile)
	_infile.clear();
	_infile.seekg(0, std::ios::beg);
	buf = '\0';
	while (_infile.get(buf)) {
		_outfile << _codebook[buf];
	}
}

void Huffman::encode() {
	// Create a priority queue sorted by frequency
	// Priority queue<data type, container, comparison>
	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	std::map<char, __int64_t>::iterator iter;
	for (iter = _frequency.begin(); iter != _frequency.end(); iter++) {
		pq.push(new Node(iter->first, iter->second));
	}
	// std::cout << "++++++ PriorityQueue ++++++" << std::endl;
	// while (!pq.empty()) {
	// 	std::cout << pq.top()->symbol << " " << pq.top()->freq << std::endl;
	// 	pq.pop();
	// }
	// std::cout << "++++++ PriorityQueue ++++++" << std::endl;

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
	Node *root = pq.top();
	pq.pop();
	// std::cout << "+++++++ Binary Tree +++++++" << std::endl;

	// Traverse the tree and assign codes
	std::string code = "";
	traverse(root, code);
	// std::cout << "I'm out." << std::endl;
}

void Huffman::traverse(Node *node, std::string code) {
	if (node->left == NULL && node->right == NULL) {
		_codebook[node->symbol] = code;
		return;
	}
	traverse(node->left, code + "0");
	traverse(node->right, code + "1");
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

	// Read header
	std::string header = "";
	std::getline(_infile, header);
	std::stringstream ss(header);
	char c = '\0';
	std::string code = "";
	while (ss >> c) {
		if (c == '\n') {
			break;
		}
		ss >> code;
		_codebook[(int)c] = code;
	}

	// Huffman decoding
	// decode();

	_infile.close();
	_outfile.close();
}

// binary tree가 있을 때 decoding code
// binary들이 띄어쓰기 없이 붙어있어서 헤더를 읽어와도 변환이 안 됨
// 어케하는겨!!!!!!!!!!!!!!!!!!

// void Huffman::decode() {
// 	Node *node = _root;
// 	char bit = '\0';
// 	while (_infile.get(bit)) {
// 		if (bit == '0') {
// 			node = node->left;
// 		} else {
// 			node = node->right;
// 		}
// 		if (node->left == NULL && node->right == NULL) {
// 			_outfile << node->symbol;
// 			node = _root;
// 		}
// 	}
// }
