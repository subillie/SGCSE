#include "fmt.h"

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