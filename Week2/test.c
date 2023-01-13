#include <stdio.h>

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
        CountDigits(N, num);
        Print(num);
    }
    return 0;
}