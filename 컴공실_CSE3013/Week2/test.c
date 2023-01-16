#include <stdio.h>

void CountDigits(int N, int* num)
{
    int i, j, rightDigit = 0;
    printf("Count1 : ");
    for (i = 1; N > 0; i *= 10)
    {
        int remainder = N % 10;
        N /= 10;
        for (j = 0; j < remainder; j++) // from 0 to 'remainder - 1'
            num[j] += (N + 1) * i;
        num[remainder] += N * i + rightDigit + 1; // same w/ below (N * i) + the right digit + 1
        for (j = remainder + 1; j < 10; j++) // from 'remainder + 1' to 9
            num[j] += N * i;

        printf("%d ", i);
        num[0] -= i; // the digit '0' was overcounted
        rightDigit += remainder * i; // for the next calculation of 'num[remainder]'
    }
    printf("\n");
}

void CountDigits2(int N, int* num)
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

        rightDigit += remainder * i; // for the next calculation of 'num[remainder]'
    }

    printf("Count2 : ");
    while (i > 0) // the digit '0' has been overcounted
    {
        i /= 10;
        num[0] -= i;
        printf("%d ", i);
    }
    printf("\n");
}

void Print(int *num)
{
    for (int i = 0; i < 10; i++)
        printf("%d ", num[i]);
    printf("\n");
}

int main()
{
    int T, N;
    scanf("%d", &T);
    for (int i = 0; i < T; i++)
    {
        scanf("%d", &N);
        int num[10] = {0,};
        int num2[10] = {0,};
        CountDigits(N, num);
        CountDigits2(N, num2);
        printf("print1 : ");
        Print(num);
        printf("print2 : ");
        Print(num2);
        printf("\n");
    }
    return 0;
}