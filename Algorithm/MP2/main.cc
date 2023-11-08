#include "Sort.h"

int *parse(char *filename, int size);

int main(int ac, char *av[]) {

	if (ac != 3) {
		std::cerr << "Usage: ./mp2_20200422 [input file] [algorithm index]" << std::endl;
		exit(1);
	}
	int algorithm = atoi(av[2]);
	if (!(1 <= algorithm && algorithm <= 4)) {
		std::cerr << "Usage: ./mp2_20200422 [input file] [algorithm index]" << std::endl;
		exit(1);
	}

	Sort* sort = new Sort(av);
	if (algorithm == 1) {
		sort->insertion();
	} else if (algorithm == 2) {
		sort->quick(0, -1);
	} else if (algorithm == 3) {
		sort->merge(0, -1);
	} else {
		sort->optQuick(0, -1);
	}
	delete sort;
	return 0;
}
