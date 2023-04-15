#include "myshell.h"

/* Global variables */
int signal_flag = 0;
volatile sig_atomic_t pid;

/* Function prototypes */
static void noPipe(int explamation, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count);
static void isPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, FILE *fp_history, int *history_count) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	int pipe_count = 0;		// Count the number of pipes
	//pid_t pid;				// Process id

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
		if ((pid = Fork()) == 0) {	// Child runs user job
			externFunction(filename, argv, environ);
			exit(1);
		}

		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
		}
		else	// when there is background process
			printf("%d %s", pid, cmdline);

		int status;	//TODO
		wait(&status);
	}
}

static void isPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	int **fd = Malloc(sizeof(int *) * pipe_count);
	for (int i = 0; i < pipe_count; i++)
		fd[i] = Malloc(sizeof(int) * 2);

	int index = 0;
	firstPipe(explamation, bg, cmdline, argv, &index, fp_history, history_count, fd);
	int i = midPipe(explamation, pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd);
	int status = lastPipe(explamation, pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd, i);

	for (i = 0; i < pipe_count; i++)
		free(fd[i]);
	free(fd);
	// 시그널 처리
}
/* $end eval */
