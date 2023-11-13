#include "Sort.h"

Sort::Sort(char *av[]) {
	_start = clock();

	// Open the input and output files
	_infile.open(av[1]);
	if (!_infile) {
		std::cerr << "File open error" << std::endl;
		exit(1);
	}
	_outfile.open(("result_" + std::string(av[2]) + "_" + std::string(av[1])).c_str());
	if (!_outfile) {
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
	_end = clock();
	_outfile << double(_end - _start) << std::endl; // 4th line: running time in seconds
	for (int i = 0; i < _size; i++) { // 5th line: the sorted list
		_outfile << _list[i] << " ";
	}

	delete _list;
	_outfile.close();
}

void Sort::insertion() {
	for (int iter = 1; iter < _size; iter++) {
		int i, key = _list[iter];
		// Find the position to insert
		for (i = iter - 1; (i >= 0 && key < _list[i]); i--) {
			_list[i + 1] = _list[i];
		}
		_list[i + 1] = key;
	}
}

void Sort::quick(int left, int right) {
	if (right == -1) {
		right = _size - 1;
	}
	if (left < right && right - left > 1) {
		int pivot = _partition(left, right);
		quick(left, pivot - 1);
		quick(pivot + 1, right);
	}
}

int Sort::_partition(int left, int right) {
	int pivot = _list[(left + right) / 2];
	int l = left - 1;
	int r = right + 1;
	while (true) {
		while (_list[++l] < pivot);
		while (_list[--r] > pivot);
		if (l >= r) {
			return r;
		}
		_swap(l, r);
	}
	return pivot;
}

void Sort::_swap(int a, int b) {
	int tmp = _list[a];
	_list[a] = _list[b];
	_list[b] = tmp;
}

void Sort::merge(int left, int right) {
	// Set the right boundary to the last index if it is not given
	if (right == -1) {
		right = _size - 1;
	}
	// Base case
	if (left >= right) {
		return;
	}

	// Devide the list
	int mid = (left + right) / 2;
	merge(left, mid);
	merge(mid + 1, right);

	// Merge the list
	_conquer(left, mid, right);
}

void Sort::_conquer(int left, int mid, int right) {
	int l, r, iter = left;
	int sizeL = mid - left + 1;
	int sizeR = right - mid;
	int *tmpL = new int[sizeL]; // tmporary list for the left side
	int *tmpR = new int[sizeR]; // tmporary list for the right side

	// Copy the list
	for (l = 0; l < sizeL; l++) {
		tmpL[l] = _list[left + l];
	}
	for (r = 0; r < sizeR; r++) {
		tmpR[r] = _list[mid + 1 + r];
	}

	// Merge the list
	l = r = 0;
	while (l < sizeL && r < sizeR) {
		if (tmpL[l] < tmpR[r]) {
			_list[iter++] = tmpL[l++];
		} else {
			_list[iter++] = tmpR[r++];
		}
	}
	while (l < sizeL) {
		_list[iter++] = tmpL[l++];
	}
	while (r < sizeR) {
		_list[iter++] = tmpR[r++];
	}

	delete [] tmpL;
	delete [] tmpR;
}

void Sort::optQuick(int left, int right) {
	// Set the right boundary to the last index if it is not given
	if (right == -1) {
		right = _size - 1;
	}
	// Use quick sort if the chunk is large
	if (left < right && right - left > OPT_NUM) {
		int pivot = _partition(left, right);
		optQuick(left, pivot);
		optQuick(pivot + 1, right);
	}
	// Use insertion sort if the chunk is small enough
	_insertion(left, right);
}

void Sort::_insertion(int left, int right) {
	int key, iter, i;

	for (iter = left + 1; iter <= right; iter++) {
		key = _list[iter];
		// Find the position to insert
		for (i = iter - 1; (i >= left && key < _list[i]); i--) {
			_list[i + 1] = _list[i];
		}
		_list[i + 1] = key;
	}
}
