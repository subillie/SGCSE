
/*
 * addNumber(set, x): Add 'x' to the bitset represented in array 'set'
 *   Do nothing if 'x' is already in the set
 *   Assume that 0 <= x <= 127 and 'set' is a pointer to 16-byte array
 */

// when x is divided by 8, the quotient is the 'byte_index'
// when x is divided by 8, the remainder is the 'bit_index'
// the 'byte_index' is the index of the byte in the bitset array
// the 'bit_index' is the index of the bit in the byte
// if the bit is already set, do nothing, otherwise, set the bit

void addNumber(unsigned char* set, int x) {

	int byte_index = x / 8;
	int bit_index = 7 - (x % 8);

	if ((set[byte_index] & (1 << bit_index)) == 0)
		set[byte_index] |= (1 << bit_index);
}

