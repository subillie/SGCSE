#include "myshell.h"

/* $begin shellmain */
int main() {

	char cmdline[MAXLINE];	/* Command line */
	t_history *history = NULL;

	while (1) {
		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline, &history);
	}
	// TODO history 로그 파일 만들기
	return 0;
}
/* $end shellmain */
