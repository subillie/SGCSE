#include "Mss.h"

int main(int ac, char* av[]) {
	int maxSum = 0;
	int algorithmIndex = atoi(av[2]);
	clock_t start, end;

	if (ac != 3 || !(1 <= algorithmIndex && algorithmIndex <= 3)) {
		std::cout << "Usage: ./mp1_20200422 <filename> <algorithm index>" << std::endl;
		return 1;
	}
	std::ofstream outfile("result_" + std::string(av[1]));
	if (!outfile) {
		std::cout << "File open error" << std::endl;
		return 1;
	}

	Mss maximumSumSubrectangle = Mss(outfile, av);

	// Find the maximum sum subrectangle
	start = clock();
	if (algorithmIndex == 1) {
		maxSum = maximumSumSubrectangle.o6();
	} else if (algorithmIndex == 2) {
		maxSum = maximumSumSubrectangle.o4();
	} else if (algorithmIndex == 3) {
		maxSum = maximumSumSubrectangle.o3();
	}
	end = clock();

	// 5th line: sum of the maximum sum subrectangle
	outfile << maxSum << std::endl;
	// 6th line: running time in milliseconds
	outfile << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	outfile.close();
	return 0;
}
