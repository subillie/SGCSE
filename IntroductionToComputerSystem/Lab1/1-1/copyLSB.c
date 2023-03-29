#include "copyLSB.h"

/*
 * copyLSB(x): return an int with all bits set to the least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 */
int copyLSB(int x) {

	return (x << 31) >> 31;
}
