//============================================================================//
// ex016: finding a pattern inside a string                                   //
//============================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nfind(char*, char*);

void main() {

  char pat[] = "wor";
  char str[] = "hello world I am a student.";
  int rv;

  rv = nfind(str, pat);
  if(rv == -1) {
    printf("The pattern %s was not found in the string.\n", pat);
  } else {
    printf("The pattern %s was found at position %d in the string.\n", pat, rv);
  }
}

int nfind(char *string, char *pat) {

  /* try implementing this function yourself! */

}


