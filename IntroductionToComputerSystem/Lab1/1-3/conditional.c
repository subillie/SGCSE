#include "conditional.h"

/*
 * conditional(x, y, z): same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 */

// if x is zero, bit_mask is initialized by '0'
// return (bit_mask & y), whick equals to y

// if x is non-zero, bit_mask is initialized by '-1'
// return (bit_mask & z), which equals to z

int conditional(int x, int y, int z) {

	int bit_mask = !x + ~0;
	return (bit_mask & y) | (~bit_mask & z);
}

