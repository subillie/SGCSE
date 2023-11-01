#include "Sort.h"

Sort::Sort(char *av[]) {
	// Open the input and output files
	_infile.open(av[1]);
	_outfile.open(("result_" + std::string(av[2]) + "_" + std::string(av[1])).c_str());
	if (!_infile || !_outfile) {
		std::cerr << "File open error" << std::endl;
		exit(1);
	}

	// Read the input file
	std::string line;
	std::getline(_infile, line);
	std::stringstream ss(line);
	ss >> _size;
	_list = new int[_size];
	if (!_list) {
		std::cerr << "Memory allocation error" << std::endl;
		exit(1);
	}

	// Parse the integer list
	int i, num;
	for (i = 0; ss >> num; i++) {
		_list[i] = num;
	}
	_infile.close();

	_outfile << av[2] << std::endl; // 1st line: input file name
	_outfile << av[1] << std::endl; // 2nd line: algorithm index
	_outfile << _size << std::endl; // 3rd line: input size (# of elements in the original list)
}

Sort::~Sort() {
	_outfile << double(_end - _start) / CLOCK_PER_SEC << std::endl; // 4th line: running time in seconds
	for (int i = 0; i < _size; i++) { // 5th line: the sorted list
		_outfile << _list[i] << " ";
	}

	// delete[] _list;
	_outfile.close();
}

void Sort::insertion() {
	int key, iter, i;

	_start = clock();
	for (iter = 1; iter < _size; iter++) {
		key = _list[iter];
		for (i = iter - 1; (i >= 0 && key < _list[i]); i--) {
			_list[i + 1] = _list[i];
		}
		_list[i + 1] = key;
	}
	_end = clock();
}

void Sort::quick() {
	_start = clock();
	_partition(0, _size - 1);
	_end = clock();
}

void Sort::_partition(int left, int right) {
	// Base case
	if (left >= right) {
		return;
	}

	// Partition the list by pivot
	int pivot = _list[right];
	int key = left - 1;
	for (int i = left; i < right; i++) {
		if (_list[i] < pivot) {
			_swap(++key, i);
		}
	}
	_swap(key + 1, right);
	pivot = key + 1;

	// Sort recursively
	_partition(left, pivot - 1);
	_partition(pivot + 1, right);
}

void Sort::_swap(int a, int b) {
	int tmp = _list[a];
	_list[a] = _list[b];
	_list[b] = tmp;
}

void Sort::merge() {
	int i, a, b;
	std::queue<int> q;

	// Copy the list to the queue
	for (i = 0; i < _size; i++) {
		q.push(_list[i]);
	}

	// Merge sort
	_start = clock();
	while (q.size() > 1) {
		a = q.front();
		q.pop();
		b = q.front();
		q.pop();

		if (a > b) {
			q.push(b);
			q.push(a);
		} else {
			q.push(a);
			q.push(b);
		}
	}
	_end = clock();

	// Copy the sorted list to the original list
	memset(_list, 0, _size * sizeof(int));
	for (i = 0; i < _size; i++) {
		_list[i] = q.front();
		q.pop();
	}
}
