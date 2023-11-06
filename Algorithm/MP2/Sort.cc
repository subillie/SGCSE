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
	_outfile << double(_end - _start) << std::endl; // 4th line: running time in seconds
	// _outfile << double(_end - _start) / CLOCK_PER_SEC << std::endl; // 4th line: running time in seconds
	for (int i = 0; i < _size; i++) { // 5th line: the sorted list
		_outfile << _list[i] << " ";
	}

	delete _list;
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
	_start = clock();
	_devide(0, _size - 1);
	_end = clock();
}

void Sort::_devide(int left, int right) {
	// Base case
	if (left >= right) {
		return;
	}

	// Devide the list
	int mid = (left + right) / 2;
	_devide(left, mid);
	_devide(mid + 1, right);

	// Merge the list
	_conquer(left, mid, right);
}

void Sort::_conquer(int left, int mid, int right) {
	int l, r, iter;
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
	iter = left;
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

void Sort::tim() {
	_start = clock();
	for (int i = 0; i < _size; i += TIM_NUM) {
		insertion();
	}
	for (int i = TIM_NUM; i < _size; i *= 2) {
		for (int left = 0; left < _size; left += 2 * i) {
			int mid = left + i - 1;
			if (mid > _size - 1) {
				mid = _size - 1;
			}
			int right = left + 2 * i - 1;
			if (right > _size - 1) {
				right = _size - 1;
			}
			_conquer(left, mid, right);
		}
	}
	_end = clock();
}
