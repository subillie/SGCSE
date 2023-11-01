//------------------------------------------------------------------------------
// Fall 2023 Design and Analysis of Algorithms
// MP2 test case generator - Jungmin So (jso1@sogang.ac.kr) 
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  if(argc != 4) {
    printf("usage: %s size max_number seed\n", argv[0]);
    return 0;
  }

  int size = atoi(argv[1]);
  int max_number = atoi(argv[2]);
  int seed = atoi(argv[3]);

  FILE *outfile;
  char outfilename[200];
  sprintf(outfilename, "input_%09d_%09d_%09d.txt", size, max_number, seed);
  outfile = fopen(outfilename, "w");
  if(outfile == NULL) {
    printf("cannot create output file\n");
    return 0;
  }

  fprintf(outfile, "%d ", size);
  
  srand(seed);
  for(int i=0; i<size; i++) {
    fprintf(outfile, "%d ", (rand()%(2*max_number)) - max_number);
  }
  fprintf(outfile, "\n");
  fclose(outfile);
  return 0;
}
  
