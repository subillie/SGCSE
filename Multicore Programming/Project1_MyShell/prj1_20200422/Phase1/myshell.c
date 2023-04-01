/* $begin shellmain */
#include "myshell.h"

/* Function prototypes */
void handler(int sig);
void eval(char *cmdline);
int builtin_command(char **argv);
int parseline(char *buf, char **argv);
void handler(int sig);

int main() {

	sigset_t mask_all, mask_one;
	char cmdline[MAXLINE];	/* Command line */

	Sigfillset(&mask_all);
	Sygemptyset(&mask_one);
	Sigaddset(&mask_one, SIGCHLD);
	Signal(SIGCHLD, handler);

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

	char *argv[MAXARGS];	/* Argument list execve() */
	char buf[MAXLINE];		/* Holds modified command line */
	int bg;					/* Should the job run in bg or fg? */
	pid_t pid;				/* Process id */

	strcpy(buf, cmdline);
	bg = parseline(buf, argv);
	if (argv[0] == NULL)	/* Ignore empty lines */
		return;
	if (!builtin_command(argv)) {					//quit -> exit(0), & -> ignore, other -> run
		if (execve(argv[0], argv, environ) < 0) {	//ex) /bin/ls ls -al &
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

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) {

	if (!strcmp(argv[0], "quit"))	/* quit command */
		exit(0);
	if (!strcmp(argv[0], "&"))		/* Ignore singleton & */
		return 1;
	return 0;						/* Not a builtin command */
}
/* $end eval */

/* $begin parseline */
void *simplify(char *cmdline, char *buf) {

	int ib, ic = -1;

	/* Replace tab with space */
	while (cmdline[++ic])
		if (cmdline[ic] == '\t')
			cmdline[ic] = ' ';

	/* Put space besides | and & */
	for (ib = 0, ic = 0; cmdline[ic]; ic++) {
		if (cmdline[ic] == '|' || \
			(cmdline[ic] == '&' && cmdline[ic - 1] != ' ' && cmdline[ic - 1] == '|')) {
			buf[ib++] = ' ';
			buf[ib++] = cmdline[ic];
			buf[ib++] = ' ';
		} else
			buf[ib++] = cmdline[ic];
	}
	buf[ib] = '\0';
	return (buf);
}

/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) {

	char *delim;	/* Points to first space delimiter */
	int argc;		/* Number of args */
	int bg;			/* Background job? */

	buf[strlen(buf)-1] = ' ';		/* Replace trailing '\n' with space */
	while (*buf && (*buf == ' '))	/* Ignore leading spaces */
		buf++;
	simplify(buf);

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))	/* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	if (argc == 0)	/* Ignore blank line */
		return 1;

	/* Should the job run in the background? */
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}
/* $end parseline */