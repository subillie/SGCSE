#include "myshell.h"

/* Function prototypes */
static void firstPipe(int explamation, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
static int midPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
static void lastPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i);

void isPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	int **fd = Malloc(sizeof(int *) * pipe_count);
	for (int i = 0; i < pipe_count; i++)
		fd[i] = Malloc(sizeof(int) * 2);

	int index = 0;
	firstPipe(explamation, bg, cmdline, argv, &index, fp_history, history_count, fd);
	int i = midPipe(explamation, pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd);
	lastPipe(explamation, pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd, i);
	initSignal();
	for (i = 0; i < pipe_count; i++)
		free(fd[i]);
	free(fd);
}

static void	firstPipe(int explamation, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

	int pipe_i = 0;
	char **tmp = argv + (*index);
	for (; (tmp[pipe_i] && tmp[pipe_i][0] != '|'); pipe_i++);

	if (pipe(fd[0]) == -1) {
		unix_error("Pipe error");
		return ;
	}

	pid_t pid = Fork();
	if (pid == 0) {			// child process
		Signal(SIGINT, SIG_DFL);
		Signal(SIGTSTP, SIG_DFL);
		close(fd[0][0]);
		dup2(fd[0][1], STDOUT_FILENO);
		close(fd[0][1]);

		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
		if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, tmp[0]);
			tmp[pipe_i] = NULL;
			externFunction(filename, tmp, environ);
		}

		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
			setpgid(pid, 0);				// Set the process to a new process group
			tcsetpgrp(STDERR_FILENO, pid);	// Pass terminal control to the child process
			while (pipe_flag == 0)			// Wait until SIGCHLD occurs
				Sigsuspend(&prev_one);
			pipe_flag = 0;
			Sigprocmask(SIG_SETMASK, &prev_one, NULL);	// Unblock SIGCHLD
			pid = getpgrp();
			tcsetpgrp(STDERR_FILENO, pid);

		// when there is background process
		} else
			printf("%d %s", pid, cmdline);

		exit(0);
	} else if (pid > 0) {	// parent process
		close(fd[0][1]);
		*index += pipe_i + 1;
	}
}

static int	midPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

	int pipe_i = 0;
	char **tmp = argv + (*index);
	for (; (tmp[pipe_i] && tmp[pipe_i][0] != '|'); pipe_i++);

	int i = 0;
	while (i < pipe_count - 1) {
		i++;
		if (pipe(fd[i]) == -1) {
			unix_error("Pipe error");
			return (-1);
		}
		pid_t pid = Fork();
		if (pid == 0) {			// child process
			Signal(SIGINT, SIG_DFL);
			Signal(SIGTSTP, SIG_DFL);
			close(fd[i][0]);
			dup2(fd[i - 1][0], STDIN_FILENO);
			close(fd[i - 1][0]);
			dup2(fd[i][1], STDOUT_FILENO);
			close(fd[i][1]);

			Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
			if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
				char *filename = (char *)malloc(sizeof(char) * MAXLINE);
				strcpy(filename, tmp[0]);
				tmp[pipe_i] = NULL;
				externFunction(filename, tmp, environ);
			}

			// Parent waits for foreground job to terminate
			if (!bg) {
				int status;
				setpgid(pid, 0);				// Set the process to a new process group
				tcsetpgrp(STDERR_FILENO, pid);	// Pass terminal control to the child process
				while (pipe_flag == 0)			// Wait until SIGCHLD occurs
					Sigsuspend(&prev_one);
				pipe_flag = 0;
				Sigprocmask(SIG_SETMASK, &prev_one, NULL);	// Unblock SIGCHLD
				pid = getpgrp();
				tcsetpgrp(STDERR_FILENO, pid);

			// when there is background process
			} else
				printf("%d %s", pid, cmdline);

			exit(0);
		} else if (pid > 0) {	// parent process
			close(fd[i - 1][0]);
			close(fd[i][1]);
			*index += pipe_i + 1;
		} else {
			return (-1);
		}
	}
	return (i);
}

static void	lastPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i) {

	int pipe_i = 0;
	char **tmp = argv + (*index);
	for (; (tmp[pipe_i] && tmp[pipe_i][0] != '|'); pipe_i++);

	int status = 0;
	pid_t pid = Fork();
	printf("pid : %d\n", pid);
	if (pid == 0) {			// child process
		Signal(SIGINT, SIG_DFL);
		Signal(SIGTSTP, SIG_DFL);
		dup2(fd[i][0], STDIN_FILENO);
		close(fd[i][0]);

		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);	// Block SIGCHLD
		if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, tmp[0]);
			tmp[pipe_i] = NULL;
			externFunction(filename, tmp, environ);
		}

		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
			setpgid(pid, 0);				// Set the process to a new process group
			tcsetpgrp(STDERR_FILENO, pid);	// Pass terminal control to the child process
			while (pipe_flag == 0)			// Wait until SIGCHLD occurs
				Sigsuspend(&prev_one);
			pipe_flag = 0;
			Sigprocmask(SIG_SETMASK, &prev_one, NULL);	// Unblock SIGCHLD
			pid = getpgrp();
			tcsetpgrp(STDERR_FILENO, pid);

		// when there is background process
		} else
			printf("%d %s", pid, cmdline);

		exit(0);
	} else if (pid > 0) {	// parent process
		close(fd[i][0]);
		i = -1;
		printf("count: %d\n", pipe_count);
		while (++i < pipe_count + 1) {
			int tmp;
			if (pid == wait(&tmp))
				status = tmp;
		}
	}
}
