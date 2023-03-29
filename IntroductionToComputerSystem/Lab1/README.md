### From 1-1 to 1-3  
There are several constraints that your code must satisfy.  
```
- Allowed operators: ! ~ & ^ | + << >>
	- Don’t use other operators such as && || - == < > ?
- Write straight-line code
	- Don’t use any control constructs such as if, do, while, for, switch, etc.
- Do not include any additional header file
- Do not declare or call any function in your code
```  

`Problem 1-1 (copyLSB.c)` :  
###### e.g. copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000  
**copyLSB(x)** : return an int with all bits set to the least significant bit of x

`Problem 1-2 (absVal.c)` :  
###### e.g. absVal(-1) = 1  
**absVal(x)** : return the absolute value of x (assume –SMAX ≤ x ≤ SMAX)

`Problem 1-3 (conditional.c)` :  
###### e.g. conditional(2,4,5) = 4  
**conditional(x, y, z)** : return the result of ternary operation “x ? y : z” in C

### For 1-4  
There is no constraint on the code; just focus on the functionality.  

`Problem 1-4 (bitset.c)` :  
###### Assume that 0 <= x <= 127 and 'set' is a pointer to 16-byte array
**addNumber(set, x)** : Add 'x' to the bitset represented in array 'set'