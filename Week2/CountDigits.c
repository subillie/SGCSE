#include "Header.h"

void CountDigits(int N, int *num)
{
    int i, j;

    for (i = 1; N > 0; i *= 10)
    {
        int remainder = N % 10;
        N /= 10;
        for (j = 0; j <= remainder; j++) // from 0 to 'remainder'
            num[j] += (N + 1) * i;
        for (j = remainder + 1; j < 10; j++) // from 'remainder + 1' to 9
            num[j] += N * i;
    }

    while (i > 0) // the digit '0' has been overcounted
    {
        i /= 10;
        num[0] -= i;
    }
}