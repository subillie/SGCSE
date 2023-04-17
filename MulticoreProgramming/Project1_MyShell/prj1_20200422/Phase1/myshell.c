#include "myshell.h"

/* $begin shellmain */
int main() {

	FILE *fp_history = Fopen("history.txt", "a+");
	fseek(fp_history, 0L, SEEK_SET);
	char line[MAXLINE];
	int history_count = 0;
	for (; fgets(line, MAXLINE, fp_history); history_count++);

	initSignal();
	char cmdline[MAXLINE];	// Command line
	while (1) {
		signal_flag = 0;

		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		if (signal_flag == 2)
			continue;

		/* Evaluate */
		eval(cmdline, fp_history, &history_count);
	}
	Fclose(fp_history);

	return 0;
}
/* $end shellmain */
