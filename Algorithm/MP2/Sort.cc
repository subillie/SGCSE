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

/* int max, min값 처리... -> radix sort도 고려해보기 */
void Sort::counting() {
	int max = _getMax();
	int min = _getMin();

	_start = clock();
	if (min >= 0) {
		_countPosNum(max);
	} else if (max <= 0) {
		_countNegNum(min);
	} else {
		_countPosNum(max);
		_countNegNum(min);
	}
	_end = clock();
}

int Sort::_getMax() {
	int max = _list[0];
	for (int i = 1; i < _size; i++) {
		if (max < _list[i]) {
			max = _list[i];
		}
	}
	return max;
}

int Sort::_getMin() {
	int min = _list[0];
	for (int i = 1; i < _size; i++) {
		if (min > _list[i]) {
			min = _list[i];
		}
	}
	return min;
}

void Sort::_countPosNum(int max) {
	int i, j, k;
	int *count = new int[max];
	int *countSum = new int[_size];
	int *sorted = new int[_size];

	// Initialize the count array
	for (i = 0; i < _size; i++) {
		count[i] = 0;
	}
	// Count the number of each element
	for (i = 0; i < _size; i++) {
		count[_list[i]]++;
	}
	// Calculate the sum of the count array
	countSum[0] = count[0];
	for (i = 1; i < max; i++) {
		countSum[i] = countSum[i - 1] + count[i];
	}
	// Sort the list
	for (i = 0; i < _size; i++) {
		sorted[countSum[_list[i]] - 1] = _list[i];
		countSum[_list[i]]--;
	}
	// Copy the sorted list
	for (i = 0; i < _size; i++) {
		_list[i] = sorted[i];
	}

	delete [] count;
	delete [] countSum;
	delete [] sorted;
}

void Sort::_countNegNum(int min) {
	int i, j, k, max = -1 * min;
	int *count = new int[max];
	int *countSum = new int[_size];
	int *sorted = new int[_size];

	// Initialize the count array
	for (i = 0; i < _size; i++) {
		count[i] = 0;
	}
	// Count the number of each element
	for (i = 0; i < _size; i++) {
		count[_list[i]]++;
	}
	// Calculate the sum of the count array
	countSum[0] = count[0];
	for (i = 1; i < max; i++) {
		countSum[i] = countSum[i - 1] + count[i];
	}
	// Sort the list
	for (i = 0; i < _size; i++) {
		sorted[countSum[_list[i]] - 1] = _list[i];
		countSum[_list[i]]--;
	}
	// Copy the sorted list
	for (i = 0; i < _size; i++) {
		_list[i] = sorted[i];
	}

	delete [] count;
	delete [] countSum;
	delete [] sorted;
}
