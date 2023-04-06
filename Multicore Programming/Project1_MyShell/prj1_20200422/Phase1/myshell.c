/* $begin shellmain */
#include "myshell.h"

/* Function prototypes */
void handler(int sig);


int main() {

	sigset_t mask_all, mask_one;
	char cmdline[MAXLINE];	/* Command line */

	// Sigfillset(&mask_all);
	// Sygemptyset(&mask_one);
	// Sigaddset(&mask_one, SIGCHLD);
	// Signal(SIGCHLD, handler);

	while (1) {
		/* Read */
		printf("CSE4100-MP-P1> ");
		fgets(cmdline, MAXLINE, stdin);
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline);
	}
	return 0;
}
/* $end shellmain */

/* handler - Handle signal */
void handler(int sig) {
	// int olderrno = errno;
	// pid_t pid;
	// int status;

	// while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
	// 	printf("Job [%d] (%d) terminated by signal %d\n", pid2jid(pid), pid, sig);
	// 	deletejob(jobs, pid);
	// }
	// if (errno != ECHILD)
	// 	unix_error("waitpid error");
	// errno = olderrno;
}
