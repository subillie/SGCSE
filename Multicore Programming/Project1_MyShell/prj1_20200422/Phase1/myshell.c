#include "myshell.h"

/* $begin shellmain */
int main() {

	char cmdline[MAXLINE];	// Command line
	FILE *fp_history = fopen("history.txt", "w");

	while (1) {
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);	// Read
		if (feof(stdin))
			exit(0);
		eval(cmdline, fp_history);		// Evaluate
	}
	fclose(fp_history);

	return 0;
}
/* $end shellmain */
