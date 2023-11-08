#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <queue>
#include <stdlib.h>

#define OPT_NUM 32

class Sort {
	private:
		std::ifstream _infile;
		std::ofstream _outfile;
		clock_t _start, _end;
		int _size;
		int *_list;
		void _parse();
		int _partition(int left, int right);
		void _swap(int a, int b);
		void _conquer(int left, int mid, int right);
		void _insertion(int left, int right);

	public:
		Sort(char *av[]);
		~Sort();
		void insertion();
		void quick(int left, int right);
		void merge(int left, int right);
		void optQuick(int left, int right);
};

#endif