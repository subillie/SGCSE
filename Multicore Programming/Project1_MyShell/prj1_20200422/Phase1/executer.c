#include "myshell.h"

/* Extern values */
int signal_flag = 0;

/* Function prototypes */
static void nonBuiltin(char *filename, char *argv[], char *environ[]);
static void handler(int sig);
static int builtinCommand(char **argv);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	pid_t pid;				// Process id
	sigset_t mask_all, mask_one, prev_one;

	Sigfillset(&mask_all);
	Sigemptyset(&mask_one);
	Sigaddset(&mask_one, SIGCHLD);
	Signal(SIGCHLD, handler);

	bg = parseline(cmdline, buf, argv);
	if (argv[0] == NULL)	/* Ignore empty lines */
		return;

	/* quit -> exit(0), & -> ignore, other -> run */
	if (!builtinCommand(argv)) {
		char bin_argv[MAXARGS] = "/bin/";
		strcat(bin_argv, argv[0]);
		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
		nonBuiltin(argv[0], argv, environ);	// e.g. /bin/ls ls -al &

		/* Parent waits for foreground job to terminate */
		if (!bg){ 
			int status;
		} else	/* when there is background process */
			printf("%d %s", pid, cmdline);
	}
	return;
}

static void nonBuiltin(char *filename, char *argv[], char *environ[]) {

	if (execve(filename, argv, environ) < 0) {
		strcpy(filename, "/bin/");
		strcat(filename, argv[0]);
		if (execve(filename, argv, environ) < 0) {
			strcpy(filename, "/usr/bin/");
			strcat(filename, argv[0]);
			if (execve(filename, argv, environ) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit (0);
			}
		}
	}
}

/* handler - Handle signal */
static void handler(int sig) {

	int olderrno = errno;
	int status;
	pid_t pid;
	sigset_t mask_all, prev;

	// WNOHANG, WUNTRACED:
	// return immediately if no child has exited, and also return if a child has stopped
	Sigfillset(&mask_all);
	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		Sigprocmask(SIG_BLOCK, &mask_all, &prev);
		// If child process is stopped or terminated, set signal_flag
		if(WIFSTOPPED(status) || WIFEXITED(status) || WIFSIGNALED(status))
			signal_flag = 1;
		Sigprocmask(SIG_SETMASK, &prev, NULL);
	}
	if (errno != ECHILD)
		unix_error("waitpid error\n");
	errno = olderrno;
}

/* If opening_q arg is a builtin command, run it and return true */
static int builtinCommand(char **argv) {

	if (!strcmp(argv[0], "exit"))		// exit command
		exit(0);
	if (!strcmp(argv[0], "&"))			// Ignore singleton &
		return 1;
	if (!strcmp(argv[0], "cd")) {		// cd command
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
	if (!strcmp(argv[0], "mkdir")) {	// mkdir command
		if (argv[1] == NULL)
			printf("usage: mkdir [-pv] [-m mode] directory ...\n");
		else if (mkdir(argv[1], 0755) < 0)
			printf("mkdir: %s: File exists\n", argv[1]);
		return 1;
	}
	if (!strcmp(argv[0], "rmdir")) {	// rmdir command
		if (argv[1] == NULL)
			printf("usage: rmdir [-p] directory ...\n");
		else if (rmdir(argv[1]) < 0)
			printf("rmdir: %s: No such file or directory\n", argv[1]);
		return 1;
	}
	if (!strcmp(argv[0], "touch")) {	// touch command
		if (argv[1] == NULL)
			printf("usage:\ntouch [-A [-][[hh]mm]SS] [-acfhm] [-r file] [-t [[CC]YY]MMDDhhmm[.SS]] file ...\n");
		else
			creat(argv[1], 0644);
		return 1;
	}
	if (!strcmp(argv[0], "cat")) {		// cat command
		if (argv[1] == NULL)
			printf("cat: missing operand\n");
		else {
			int fd = open(argv[1], O_RDONLY);
			if (fd < 0) {
				printf("cat: %s: No such file or directory\n", argv[1]);
				return 1;
			}
			char buf[MAXLINE];
			int n;
			while ((n = read(fd, buf, MAXLINE)) > 0)
				write(1, buf, n);
			close(fd);
		}
		return 1;
	}
	if (!strcmp(argv[0], "echo")) {		// echo command
		int i = 1;
		while (argv[i] != NULL) {
			printf("%s ", argv[i]);
			i++;
		}
		printf("\n");
		return 1;
	}
	if (!strcmp(argv[0], "history")) {
		if (argv[1] == NULL) {
			printf("history: missing operand\n");
			return 1;
		}
		else {
			HISTORY_STATE *history_info = history_get_history_state();
			HIST_ENTRY **my_history = history_list();
			for (int i = 0; i < history_info->length; i++) {
				printf("%d  %s\n", i + 1, my_history[i]->line);
				free_history_entry(my_history[i]);
			}
			free(history_info);
			free(my_history);
		}
	}
	return 0;							// Not a builtin command
}

    /* get the state of your history list (offset, length, size) */
    // HISTORY_STATE *myhist = history_get_history_state ();

    // /* retrieve the history list */
    // HIST_ENTRY **mylist = history_list ();

    // printf ("\nsession history for %s@%s\n\n", p, host);
    // for (int i = 0; i < myhist->length; i++) { /* output history list */
    //     printf (" %8s  %s\n", mylist[i]->line, mylist[i]->timestamp);
    //     free_history_entry (mylist[i]);     /* free allocated entries */
    // }
    // putchar ('\n');

    // free (myhist);  /* free HIST_ENTRY list */
    // free (mylist);  /* free HISTORY_STATE   */

/* $end eval */
