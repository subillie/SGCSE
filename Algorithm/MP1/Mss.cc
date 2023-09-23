#include "Mss.h"

Mss::Mss(std::ofstream& outfile, char* av[]) {
	int num;
	std::string line;
	std::ifstream infile(av[1]);
	if (!infile) {
		std::cout << "File open error" << std::endl;
		exit(1);
	}

	outfile << av[1] << std::endl; // 1st line: input file name
	outfile << av[2] << std::endl; // 2nd line: algorithm index

	// Read the first line
	std::getline(infile, line);
	std::stringstream ss(line);
	ss >> _row >> _col;
	outfile << _row << std::endl; // 3rd line: # of rows in the given 2D array
	outfile << _col << std::endl; // 4th line: # of columns in the given 2D array
	line.clear();
	ss.clear();

	// Read the rest of the infile
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
	_array.clear();
}

int Mss::o6() {
	int maxSum = 0;
	int thisSum = 0;

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
					if (maxSum < thisSum) {
						maxSum = thisSum;
					}
				}
			}
		}
	}

	return maxSum;
}

int Mss::o4() {
	int maxSum = 0;
	int thisSum = 0;

	for (int r = 0; r < _row; r++) {
		for (int c = 0; c < _col; c++) {
			thisSum = 0;
			for (int y = r; y < _row; y++) {
				for (int x = c; x < _col; x++) {
					thisSum += _array[y][x];
				}
				if (maxSum < thisSum) {
					maxSum = thisSum;
				}
			}
		}
	}

	return maxSum;
}

int Mss::o3() {
	int maxSum = 0;
	int thisSum = 0;

	return maxSum;
}
