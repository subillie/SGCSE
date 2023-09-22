#ifndef MSS_H
#define MSS_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>

class Mss {
	private:
		std::vector< std::vector<int> > _array;
		int _row, _col;
	public:
		Mss(std::ofstream& outfile, char* av[]);
		~Mss();
		int o6();
		int o4();
		int o3();
};

#endif
