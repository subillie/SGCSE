#include "Header.h"

int main()
{
    int T, N;
    scanf("%d", &T);
    for (int i = 0; i < T; i++)
    {
        scanf("%d", &N);
        int num[10] = {0,};
        countDigits(N, num);
        print(num);
    }
    return 0;
}