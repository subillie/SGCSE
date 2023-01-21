#include "fmt.h"

void outputL1(char *L1, int *Count, int *B_Flag)
{
    int i, longWordFlag = 0;
    int Start = 0, N_Blanks, N_Chars;

    // Start는 단어의 끝을 가리킴 (현재 이 인덱스까지 글자의 출력이 진행되어 왔음을 의미)
    // N_Blanks는 Start 이후 존재하는 blank 글자 수
    // N_Chars는 다음 단어의 글자 수
    Get_Blanks_Chars(L1, Start, &N_Blanks, &N_Chars);
    while (N_Chars)
    {
        // LIMIT보다 작으면서 B_Flag가 1인 경우 (print at the same line)
        if (((*Count + N_Chars + 1) <= LIMIT) && (*B_Flag == 1))
        {
            if (N_Blanks)
            {
                printf("Output Error");
                exit(1);
            }
            putchar(' ');
            for (i = Start; i < Start + N_Chars; i++)
                putchar(L1[i]);
            Start += N_Chars;
            *Count += (N_Chars + 1);
            *B_Flag = 0;
        }

        // LIMIT보다 작으면서 B_Flag가 0인 경우 (print at the next line)
        else if (((*Count + N_Chars + 1) <= LIMIT))
        {
            for (i = Start; i < Start + N_Blanks + N_Chars; i++)
                putchar(L1[i]);
            Start += (N_Blanks + N_Chars);
            *Count += (N_Blanks + N_Chars);
        }

        // LIMIT보다 큰 경우
        else
        {
            if (*Count) // print at the next line
            {
                putchar('\n');
                for (i = Start + N_Blanks; i < Start + N_Blanks + N_Chars; i++)
                    putchar(L1[i]);
                Start += (N_Blanks + N_Chars);
                *Count = N_Chars;
            }
            else // print all at the current line
            {
                longWordFlag = 1;
                for (i = Start; i < Start + N_Blanks + N_Chars; i++)
                    putchar(L1[i]);
                putchar('\n');
                Start += (N_Blanks + N_Chars);
                Get_Blanks_Chars(L1, Start, &N_Blanks, &N_Chars);
                Start += N_Blanks;
            }
        }

        if (!longWordFlag)
            Get_Blanks_Chars(L1, Start, &N_Blanks, &N_Chars);
        else
            longWordFlag = 0;
    }
}