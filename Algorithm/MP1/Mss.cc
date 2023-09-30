#include "Mss.h"

Mss::Mss(char* av[]) {
	std::ifstream infile(av[1]);
	_outfile.open(("result_" + std::string(av[1])).c_str());
	if (!infile || !_outfile) {
		std::cout << "File open error" << std::endl;
		exit(1);
	}

	_outfile << av[1] << std::endl; // 1st line: input file name
	_outfile << av[2] << std::endl; // 2nd line: algorithm index

	// Read the first line
	std::string line;
	std::getline(infile, line);
	std::stringstream ss(line);
	ss >> _row >> _col;
	_outfile << _row << std::endl; // 3rd line: # of rows in the given 2D array
	_outfile << _col << std::endl; // 4th line: # of columns in the given 2D array
	line.clear();
	ss.clear();

	// Read the rest of the infile
	int num;
	for (int i = 0; i < _row; i++) {
		std::vector<int> row;
		std::getline(infile, line);
		std::stringstream ss(line);
		for (int j = 0; j < _col; j++) {
			ss >> num;
			row.push_back(num);
		}
		_array.push_back(row);
		row.clear();
		ss.clear();
	}
	infile.close();
}

Mss::~Mss() {
	_outfile << _maxSum << std::endl; // 5th line: sum of the maximum sum subrectangle
	_outfile << (double)(_end - _start) / CLOCKS_PER_MSEC << std::endl; // 6th line: running time in milliseconds
	_outfile.close();
	_array.clear();
}

void Mss::o6() {
	int thisSum = 0;

	_maxSum = 0;
	_start = clock();
	for (int r1 = 0; r1 < _row; r1++) {
		for (int c1 = 0; c1 < _col; c1++) {
			for (int r2 = r1; r2 < _row; r2++) {
				for (int c2 = c1; c2 < _col; c2++) {
					thisSum = 0;
					for (int x = c1; x <= c2; x++) {
						for (int y = r1; y <= r2; y++) {
							thisSum += _array[y][x];
						}
					}
					if (_maxSum < thisSum) {
						_maxSum = thisSum;
					}
				}
			}
		}
	}
	_end = clock();
}

void Mss::o4() {
	int thisSum = 0;
	int *lineSum = new int[_col];

	_maxSum = 0;
	_start = clock();
	for (int r1 = 0; r1 < _row; r1++) {
		for (int r2 = r1; r2 < _row; r2++) {
			memset(lineSum, 0, sizeof(int) * _col);
			for (int c = 0; c < _col; c++) {
				for (int r = r1; r <= r2; r++) {
					lineSum[c] += _array[r][c];
				}
			}
			thisSum = 0;
			for (int c = 0; c < _col; c++) {
				thisSum = std::max(lineSum[c], thisSum + lineSum[c]);
				if (_maxSum < thisSum) {
					_maxSum = thisSum;
				}
			}
		}
	}
	_end = clock();
}

void Mss::o3() {
	int thisSum = 0;
	int* lineSum = new int[_col];

	_maxSum = 0;
	_start = clock();
	for (int r1 = 0; r1 < _row; r1++) {
		memset(lineSum, 0, sizeof(int) * _col);
		for (int r2 = r1; r2 < _row; r2++) {
			for (int c = 0; c < _col; c++) {
				lineSum[c] += _array[r2][c];
			}
			thisSum = 0;
			for (int c = 0; c < _col; c++) {
				thisSum = std::max(lineSum[c], thisSum + lineSum[c]);
				if (_maxSum < thisSum) {
					_maxSum = thisSum;
				}
			}
		}
	}
	_end = clock();
}
