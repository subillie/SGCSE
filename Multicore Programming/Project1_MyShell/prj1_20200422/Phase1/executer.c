#include "myshell.h"

/* Function prototypes */
static int builtin_command(char **argv);

/* $begin eval */
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
static int builtin_command(char **argv) {

	if (!strcmp(argv[0], "quit"))	// quit command
		exit(0);
	if (!strcmp(argv[0], "&"))		// Ignore singleton &
		return 1;
	if (!strcmp(argv[0], "cd")) {	// cd command
		if (argv[1] == NULL)
			chdir(getenv("HOME"));
		else if (chdir(argv[1]) < 0) {
			if (!argv[2])
				printf("cd: no such file or directory:%s\n", argv[1]);
			else {
				printf("cd : string not in pwd:");
				for (int i = 2; argv[i]; i++)
					printf(" %s", argv[i]);
				printf("\n");
			}
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
