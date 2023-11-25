#include "Huffman.h"

Huffman::Huffman() {
	_root = NULL;
	_buf = NULL;
	_fdIf = -1;
	_fdOf = -1;
	for (int i = 0; i < 256; i++) {
		_codes[i] = "";
	}
	for (int i = 0; i < 256; i++) {
		_frequency[i] = 0;
	}
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

	char buf;
	while (get(_infile, buf)) {
		_frequency[buf]++;
	}
	encode();

	_infile.close();
	_outfile.close();
}

// void Huffman::compress(std::string input) {
// 	_fdIf = open(input.c_str(), O_RDONLY);
// 	if (_fdIf == -1) {
// 		std::cerr << "File open error" << std::endl;
// 		return;
// 	}
// 	_fdOf = open((input + ".zz").c_str(), O_WRONLY | O_CREAT, 0644);
// 	if (!_fdOf == -1) {
// 		std::cerr << "File open error" << std::endl;
// 		close(_fdIf);
// 		return;
// 	}

// 	_buf = NULL;
// 	std::stringstream chunk;
// 	int count = 0;
// 	while (read(_fdIf, _buf, 1) > 0) {
// 		// Add to stringstream
// 		if (count < CHUNK_SIZE) {
// 			chunk << _buf;
// 			count++;
// 		// Encode the chunk and reset stringstream and count
// 		} else {
// 			encode(chunk.str());
// 			chunk.str("");
// 			count = 0;
// 		}
// 	}
// 	encode(chunk.str());

// 	close(_fdIf);
// 	close(_fdOf);
// }

void Huffman::encode(std::string line) {
	std::priority_queue<Node *, std::vector<Node *>, Node> pq;
	for (auto iter = _frequency.begin(); iter != _frequency.end(); iter++) {
		pq.push(new Node(iter->first, iter->second));
	}

	while (pq.size() != 1) {
		Node *tmpL = pq.top();
		pq.pop();
		Node *tmpR = pq.top();
		pq.pop();

		Node *parent = new Node(tmpL->frequency + tmpR->frequency);
		parent->left = tmpL;
		parent->right = tmpR;
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

// void Huffman::decompress(std::string input) {
// 	_infile.open(input, std::ios::binary);
// 	if (!_infile.is_open()) {
// 		std::cerr << "File open error" << std::endl;
// 		return;
// 	}
// 	_outfile.open(input + ".yy");
// 	if (!_outfile.is_open()) {
// 		std::cerr << "File open error" << std::endl;
// 		_infile.close();
// 		return;
// 	}

// 	_infile.close();
// 	_outfile.close();
// }
