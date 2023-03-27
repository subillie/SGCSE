#include "fmt.h"

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