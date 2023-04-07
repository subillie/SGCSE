#include "myshell.h"

/* Function prototypes */
void handler(int sig);

/* $begin shellmain */
int main() {

	char cmdline[MAXLINE];	/* Command line */
	using_history();

	while (1) {
		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline);

		if (strlen(cmdline) > 0)
			add_history(cmdline);
	}
	return 0;
}
/* $end shellmain */
