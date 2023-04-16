#include "myshell.h"

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, FILE *fp_history, int *history_count) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?

	if (signal_flag == 2)
		return;

	// Parse command line
	bg = parseline(cmdline, buf, argv);
	if (bg == -1)			// Ignore empty line
		return;
	int explamation = addHistory(cmdline, buf, argv, fp_history, history_count);

	// Execute command line
	if (!builtinCommand(explamation, cmdline, argv, fp_history, history_count)) {
		char *filename = (char *)malloc(sizeof(char) * MAXLINE);
		strcpy(filename, argv[0]);

		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
		if ((pid = Fork()) == 0) {	// Child runs user job
			 Sigprocmask(SIG_SETMASK, &prev_one, NULL);
			 setpgid(0, 0);			// Add the child process to the process group
			 externFunction(filename, argv, environ);
			 exit(0);
		}

		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
			setpgid(pid, 0);				// Set the process to a new process group
			tcsetpgrp(STDERR_FILENO, pid);	// Pass terminal control to the child process
			while (signal_flag == 0)		// Wait until SIGCHLD occurs
				Sigsuspend(&prev_one);
			signal_flag = 0;
			Sigprocmask(SIG_SETMASK, &prev_one, NULL);	// Unblock SIGCHLD
			pid = getpgrp();
			tcsetpgrp(STDERR_FILENO, pid);

		// When there is background process
		} else
			printf("[%d] %s", pid, cmdline);
	}

	return;
}
/* $end eval */
