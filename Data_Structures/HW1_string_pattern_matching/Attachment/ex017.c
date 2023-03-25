//============================================================================//
// ex017.c: Knuth-Morris-Pratt Algorithm for Pattern Matching                 //
//============================================================================//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define MAX_PATTERN_SIZE 100

int pmatch(char*, char*);
void fail(char*);

int failure[MAX_PATTERN_SIZE];
char string[MAX_STRING_SIZE];
char pat[MAX_PATTERN_SIZE];

int main() {

  int rv, i;
  strcpy(string, "hello world my name is world.");
  strcpy(pat, "world");
  fail(pat);
  for(i=0; i<strlen(pat); i++) {
    printf("%d ", failure[i]);
  }
  printf("\n"); 
  rv = pmatch(string, pat);
  printf("rv: %d\n", rv);
  return 0;
}

void fail(char *pat) {

  /* compute pattern's failure function */
  int i, j, n = strlen(pat);
  failure[0] = -1;
  for(j=1; j<n; j++) {
    i = failure[j-1];
    while((pat[j] != pat[i+1]) && (i >= 0)) i = failure[i];
    if(pat[j] == pat[i+1]) failure[j] = i+1;
    else failure[j] = -1;
  }
}

int pmatch(char *string, char *pat) {

  /* Knuth-Morris-Pratt string matching algorithm */
  int i=0, j=0;
  int lens = strlen(string);
  int lenp = strlen(pat);
  while(i < lens && j < lenp) {
    if(string[i] == pat[j]) { i++; j++; }
    else if(j == 0) i++;
    else j = failure[j-1] + 1;
  }
  return ((j==lenp) ? (i-lenp) : -1);
}

