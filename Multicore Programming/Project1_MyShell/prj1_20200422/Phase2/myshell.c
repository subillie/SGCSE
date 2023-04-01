#include "csapp.h"
#define MAXARGS 128

int main()
{
	char cmdline[MAXLINE];

	while (1) {
		printf("CSE4100-MP-P2> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);
		eval(cmdline);
	}
	return 0;
}