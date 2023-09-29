#include "Mss.h"

int main(int ac, char* av[]) {
	int maxSum = 0;
	int algorithmIndex = atoi(av[2]);
	clock_t start, end;

	if (ac != 3 || !(1 <= algorithmIndex && algorithmIndex <= 3)) {
		std::cout << "Usage: ./mp1_20200422 <filename> <algorithm index>" << std::endl;
		return 1;
	}

	Mss maximumSumSubrectangle(av);

	// Find the maximum sum subrectangle
	if (algorithmIndex == 1) {
		maximumSumSubrectangle.o6();
	} else if (algorithmIndex == 2) {
		maximumSumSubrectangle.o4();
	} else if (algorithmIndex == 3) {
		maximumSumSubrectangle.o3();
	}

	return 0;
}
