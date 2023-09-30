#ifndef MSS_H
#define MSS_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>

#define CLOCKS_PER_MSEC 1000

class Mss {
	private:
		std::ofstream _outfile;
		std::vector< std::vector<int> > _array;
		int _row, _col;
		int _maxSum;
		clock_t _start, _end;

	public:
		Mss(char* av[]);
		~Mss();
		void o6();
		void o4();
		void o3();
};

#endif
