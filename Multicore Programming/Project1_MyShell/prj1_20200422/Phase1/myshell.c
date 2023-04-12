#include "myshell.h"

/* $begin shellmain */
int main() {

	sigset_t mask_all, mask_one;
	char cmdline[MAXLINE];	/* Command line */
	FILE *fp_history = Fopen("history.txt", "a+");
	// using_history(); //TODO

	fseek(fp_history, 0L, SEEK_SET);
	char line[MAXLINE];
	int history_count = 0;
	for (; fgets(line, MAXLINE, fp_history); history_count++);

	while (1) {
		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);
		// add_history(cmdline); //TODO

		/* Evaluate */
		eval(cmdline, fp_history, &history_count);
	}
	Fclose(fp_history);

	return 0;
}
/* $end shellmain */
