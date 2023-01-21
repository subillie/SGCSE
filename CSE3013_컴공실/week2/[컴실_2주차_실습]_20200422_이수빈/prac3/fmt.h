#include <stdio.h>
#include <stdlib.h>

#define BNUM 256 // 한 줄을 읽을 입력 버퍼의 크기
#define LIMIT 72 // 출력 줄의 글자 수 제한

int checkHead(char *L1);
void checkTail(char *L1);
void outputL1(char *L1, int *Count, int *B_Flag);
void Get_Blanks_Chars(char *L1, int Start, int *N_Blanks, int *N_Chars);