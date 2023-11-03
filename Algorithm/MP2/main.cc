#include "Sort.h"

int *parse(char *filename, int size);

int main(int ac, char *av[]) {

	int algorithm = atoi(av[2]);
	if (ac != 3 || !(1 <= algorithm && algorithm <= 4)) {
		std::cerr << "Usage: ./mp2_20200422 [input file] [algorithm index]" << std::endl;
		exit(1);
	}

	Sort* sort = new Sort(av);
	if (algorithm == 1) {
		sort->insertion();
	} else if (algorithm == 2) {
		sort->quick();
	} else if (algorithm == 3) {
		sort->merge();
	} else {
		sort->counting();
	}
	delete sort;
	return 0;
}
