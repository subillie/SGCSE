**From 1-1 to 1-3,** there are several constraints that your code must satisfy.  
```
- Allowed operators: ! ~ & ^ | + << >>
	- Don’t use other operators such as && || - == < > ?
- Write straight-line code
	- Don’t use any control constructs such as if, do, while, for, switch, etc.
- Do not include any additional header file
- Do not declare or call any function in your code
```  

`Problem 1-1 (copyLSB.c)` :
- **copyLSB(x)** : return an int with all bits set to the least significant bit of x
- e.g. copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000  

`Problem 1-2 (absVal.c)` :
- **absVal(x)** : return the absolute value of x (assume –SMAX ≤ x ≤ SMAX)
- e.g. absVal(-1) = 1  

`Problem 1-3 (conditional.c)` :
- **conditional(x, y, z)** : return the result of ternary operation “x ? y : z” in C
- e.g. conditional(2,4,5) = 4  

**For 1-4,** there is no constraint on the code; just focus on the functionality.  

`Problem 1-4 (bitset.c)` :
- **addNumber(set, x)** : Add 'x' to the bitset represented in array 'set'
- Assume that 0 <= x <= 127 and 'set' is a pointer to 16-byte array