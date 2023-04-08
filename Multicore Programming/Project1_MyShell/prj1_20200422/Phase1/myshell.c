#include "myshell.h"

/* $begin shellmain */
int main() {

	char cmdline[MAXLINE];	// Command line
	t_history *history = NULL;

	while (1) {
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);	// Read
		if (feof(stdin))
			exit(0);
		eval(cmdline, &history);		// Evaluate
	}

	// make history log file
	FILE *fp_history = fopen("history.txt", "w");
	t_history *cur = history;
	char c = '0';
	int log_index = 0;
	while ('0' <= c && c <= '9') {
		char c = fgetc(fp_history);
		if (c == EOF)
			return 0;
		log_index *= 10;
		log_index += (c - '0');
	}
	while (cur) {
		log_index++;
		fprintf(fp_history, "%-4d%s\n", log_index, cur->cmd);
		cur = cur->next;
	}
	return 0;
}
/* $end shellmain */
