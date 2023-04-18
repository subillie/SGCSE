#include "myshell.h"

/* Function prototypes */
static void noPipe(int explamation, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, FILE *fp_history, int *history_count) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	int pipe_count = 0;		// Count the number of pipes

	// Parse command line
	bg = parseline(cmdline, buf, argv, &pipe_count);
	if (bg == -1)	// Ignore empty line
		return;
	int explamation = addHistory(cmdline, buf, argv, fp_history, history_count);

	// Execute command line
	if (pipe_count == 0)
		noPipe(explamation, bg, cmdline, argv, fp_history, history_count);
	else {
		int stdin_dup = dup(STDIN_FILENO);
		int stdout_dup = dup(STDOUT_FILENO);
		isPipe(explamation, pipe_count, bg, cmdline, argv, fp_history, history_count);
		dup2(stdin_dup, STDIN_FILENO);
		dup2(stdout_dup, STDOUT_FILENO);
		close(stdin_dup);
		close(stdout_dup);
	}

	return;
}

static void noPipe(int explamation, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	if (!builtinCommand(explamation, cmdline, argv, fp_history, history_count)) {
		// Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
		char *filename = (char *)malloc(sizeof(char) * MAXLINE);
		strcpy(filename, argv[0]);

		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
		if ((pid = Fork()) == 0) {	// Child runs user job
			 Sigprocmask(SIG_SETMASK, &prev_one, NULL);
			 setpgid(0, 0);			// Add the child to the process group
			 externFunction(filename, argv, environ);
			 exit(0);
		}
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

	// when there is background process
	} else
		printf("%d %s", pid, cmdline);

	return;
}
/* $end eval */
