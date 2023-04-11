/* $begin shellmain */
#include "myshell.h"

int main() {

	sigset_t mask_all, mask_one;
	char cmdline[MAXLINE];	/* Command line */
	FILE *fp_history = Fopen("history.txt", "w");

	while (1) {
		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline, fp_history);
	}
	Fclose(fp_history);

	return 0;
}
/* $end shellmain */
