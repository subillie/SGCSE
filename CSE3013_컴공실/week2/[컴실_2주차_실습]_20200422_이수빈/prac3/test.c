#include <stdio.h>
#include <stdlib.h>

#define BNUM 256 // 한 줄을 읽을 입력 버퍼의 크기
#define LIMIT 36 // 출력 줄의 글자 수 제한

int checkHead(char *L1)
{
    int head = 0, newlineFlag = 0;

    while ((L1[head] == ' ') || (L1[head] == '\n'))
    {
        if (L1[head] == ' ')
            head++;
        else if (L1[head] == '\n')
        {
            newlineFlag = 1;
            break;
        }
    }
    return (newlineFlag);
}

void checkTail(char *L1)
{
    int tail = 0, newlineFlag = 0;
    
    while (L1[tail])
    {
        if (L1[tail] == '\n')
        {
            newlineFlag = 1;
            break;
        }
        tail++;
    }
    tail--;

    while (L1[tail] == ' ')
        tail--;
    
    if (newlineFlag) // (R8) 끝에 개행문자가 있을 경우 개행도 출력해야 함
    {
        L1[tail + 1] = '\n';
        L1[tail + 2] = '\0';
    }
    else // (R8) 끝에 개행문자가 없으면 출력도 마찬가지로 개행을 출력하지 않음
        L1[tail + 1] = '\0';
}

void Get_Blanks_Chars(char *L1, int Start, int *N_Blanks, int *N_Chars)
{
    int flag = 0;
    *N_Blanks = 0;
    *N_Chars = 0;

    while (1)
    {
        if ((L1[Start] == '\n') || (L1[Start] == '\0'))
            break;
        else if (L1[Start] == ' ')
        {
            if (flag == 1)
                break;
            ++(*N_Blanks);
        }
        else
        {
            flag = 1;
            ++(*N_Chars);
        }
        ++Start;
    }
}

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

int main(int argc, char *argv[])
{
    FILE *fp;
    char *L1, *L2, *tmpline;
    int Count = 0, B_Flag = 0, B_Line = 0;

    if (argc != 2)
    {
        printf("Arguments Count Error\n");
        return 0;
    }
    fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("File Open Error\n");
        return 0;
    }

    L1 = (char *)malloc(sizeof(char) * BNUM);
    L2 = (char *)malloc(sizeof(char) * BNUM);
    if (!L1 || !L2)
    {
        printf("Memory Allocation Error\n");
        exit(1);
    }

    // read a line ((R9) empty file이면 아무것도 출력하지 않음)
    if ( fgets(L1, BNUM, fp) == NULL )
        return 0;

    // L1에 대한 전처리
    if (checkHead(L1)) // (R7) 빈 줄은 그대로 빈 줄로 출력
        putchar('\n');
    checkTail(L1); // (R1) 출력 각 줄의 마지막에는 blank를 출력하지 않음


    // Count는 현재 줄에 실제로 출력할 글자의 개수, 해당 행을 모두 출력하고 나면 0으로 초기화
    // B_Flag가 1이면 같은 줄에 출력, 0이면 새로운 줄에 출력
    // B_Line이 1이면 해당 라인이 empty line이라는 의미, 0이면 출력할 글자가 있는 상태
    while (1)
    {
        // L1에 대한 처리를 수행
        if (B_Line == 0)
            outputL1(L1, &Count, &B_Flag);
        else
            B_Line = 0;

        if (Count != 0)
            B_Flag = 1;

        // read the next line (EOF이면 종료)
        if ( fgets(L2, BNUM, fp) == NULL )
            break;

        // L2에 대한 전처리
        if (checkHead(L2)) // L2가 빈 줄인 경우
        {
            if (!checkHead(L1))
                putchar('\n');
            putchar('\n');
            B_Flag = 0;
            B_Line = 1;
            Count = 0;
        }
        else if (L2[0] == ' ') // (R5) L2가 빈 줄이 아니면서 공백으로 시작하는 경우
        {
            putchar('\n');
            B_Flag = 0;
            Count = 0;
        }
        checkTail(L2);

        // L2를 L1으로 복사, 단순히 포인터만 바꾸어 실제 글자 복사를 피함
        tmpline = L1;
        L1 = L2;
        L2 = tmpline;
    }

    int i = 0;
    while (L1[i] && ( L1[i] != '\n'))
        ++i;
    if (L1[i] == '\n')
        putchar('\n');

    free(L1);
    free(L2);
    return 0;
}