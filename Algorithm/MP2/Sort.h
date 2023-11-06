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

#define CLOCK_PER_SEC 1000000

class Sort {
	private:
		std::ifstream _infile;
		std::ofstream _outfile;
		clock_t _start, _end;
		int _size;
		int *_list;
		void _parse();
		void _partition(int left, int right);
		void _swap(int a, int b);
		void _devide(int left, int right);
		void _conquer(int left, int mid, int right);
		void _countingSort();
		// int _countPosNum(int min, int max);
		// void _sortPosNum(int max);
		// void _sortNegNum(int min);
		// void _sortNum(int min, int max, int count);

	public:
		Sort(char *av[]);
		~Sort();
		void insertion();
		void quick();
		void merge();
		void counting();
};

#endif