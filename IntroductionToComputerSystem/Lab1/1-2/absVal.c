#include "absVal.h"

/*
 * absVal(x): return the absolute value of x
 *   You may assume that INT_MIN (-2^31) is not provided as an input.
 *   Example: absVal(-1) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 */
int absVal(int x) {

	return (x + (x >> 31)) ^ (x >> 31);
}
