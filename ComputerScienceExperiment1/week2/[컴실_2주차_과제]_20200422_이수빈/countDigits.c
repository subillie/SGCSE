#include "Header.h"

void countDigits(int N, int* num)
{
    int i, j, rightDigit = 0;

    for (i = 1; N > 0; i *= 10)
    {
        int remainder = N % 10;
        N /= 10;
        for (j = 0; j < remainder; j++) // from 0 to 'remainder - 1'
            num[j] += (N + 1) * i;
        num[remainder] += N * i + rightDigit + 1; // same w/ below (N * i) + the right digit + 1
        for (j = remainder + 1; j < 10; j++) // from 'remainder + 1' to 9
            num[j] += N * i;

        num[0] -= i; // the digit '0' was overcounted
        rightDigit += remainder * i; // for the next calculation of 'num[remainder]'
    }
}