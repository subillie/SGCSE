/* $begin shellmain */
#include "myshell.h"

/* Function prototypes */
void handler(int sig);
void eval(char *cmdline);
int builtin_command(char **argv);
int parseline(char *cmdline, char *buf, char **argv);
void render_line(char *cmdline, char *buf);
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

/* eval - Evaluate a command line */
void eval(char *cmdline) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	pid_t pid;				// Process id

	bg = parseline(cmdline, buf, argv);
	if (argv[0] == NULL)	/* Ignore empty lines */
		return;

	/* quit -> exit(0), & -> ignore, other -> run */
	if (!builtin_command(argv)) {
		if (execve(argv[0], argv, environ) < 0) {	// e.g. /bin/ls ls -al &
			printf("%s: Command not found.\n", argv[0]);
			exit(0);
		}

		/* Parent waits for foreground job to terminate */
		if (!bg){ 
			int status;
		} else	/* when there is background process */
			printf("%d %s", pid, cmdline);
	}
	return;
}

/* If opening_q arg is a builtin command, run it and return true */
int builtin_command(char **argv) {

	if (!strcmp(argv[0], "quit"))	// quit command
		exit(0);
	if (!strcmp(argv[0], "&"))		// Ignore singleton &
		return 1;
	if (!strcmp(argv[0], "cd")) {	// cd command
		if (argv[1] == NULL) {
			chdir(getenv("HOME"));
		}
		else if (chdir(argv[1]) < 0) {
			printf("%s: No such file or directory\n", argv[1]);
			return 1;
		}
		return 1;
	}
	if (!strcmp(argv[0], "echo")) {	// echo command
		int i = 1;
		while (argv[i] != NULL) {
			printf("%s ", argv[i]);
			i++;
		}
		printf("\n");
		return 1;
	}
	return 0;						// Not a builtin command
}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *cmdline, char *buf, char **argv) {

	char *delim;	// Points to opening_q space delimiter
	int argc = 0;	// Number of args
	int bg;			// Background job?

	render_line(cmdline, buf);

	/* Build the argv list */
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))	// Ignore spaces
			buf++;
	}
	argv[argc] = NULL;

	/* Ignore blank line */
	if (argc == 0)
		return 1;

	/* Should the job run in the background? */
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}

void render_line(char *cmdline, char *buf) {

	int i_b, i_c = -1;

	strcpy(buf, cmdline);
	cmdline[strlen(cmdline)-1] = ' ';		// Replace trailing '\n' with space
	while (*cmdline && (*cmdline == ' '))	// Ignore leading spaces
		cmdline++;

	/* Replace tab with space */
	while (cmdline[++i_c])
		if (cmdline[i_c] == '\t')
			cmdline[i_c] = ' ';

	/* Put space besides | and & */
	i_b = 0, i_c = 0;
	while (cmdline[i_c] && i_b < MAXLINE - 1) {
		if (cmdline[i_c] == '|' || cmdline[i_c] == '&') {
			buf[i_b++] = ' ';
			buf[i_b++] = cmdline[i_c++];
			buf[i_b++] = ' ';
		} else
			buf[i_b++] = cmdline[i_c++];
	}
	buf[i_b] = '\0';

	/* Ignore matching quotes */
	i_b = 0;
	int inside_quote = 0;
	char quote_char = '\0';

	for (int i = 0; i < strlen(cmdline); i++) {
		if (cmdline[i] == '"' || cmdline[i] == '\'') {
			if (inside_quote && cmdline[i] == quote_char) {
				// End of quote found, do not include the quotes in the buf
				inside_quote = 0;
				quote_char = '\0';
			} else if (!inside_quote) {	// Start of quote found
				inside_quote = 1;
				quote_char = cmdline[i];
			} else
				buf[i_b++] = cmdline[i];	// Nested quotes found
		} else {
			if (inside_quote == 0)
				buf[i_b++] = cmdline[i];	// Add non-quote characters to the buf
			else
				buf[i_b++] = cmdline[i];	// Add quote characters to the buf
		}
	}

	// Add null terminator to the buf string
	buf[i_b] = '\0';

	// Copy the buf string back to the input string
	strcpy(cmdline, buf);
}
/* $end parseline */