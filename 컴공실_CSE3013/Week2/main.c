#include "Header.h"

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