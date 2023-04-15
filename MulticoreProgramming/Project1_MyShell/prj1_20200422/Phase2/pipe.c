#include "myshell.h"

void	firstPipe(int explamation, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

	int pipe_i = 0;
	char **tmp = argv + (*index);
	for (; (tmp[pipe_i] && tmp[pipe_i][0] != '|'); pipe_i++);

	if (pipe(fd[0]) == -1) {
		unix_error("Pipe error");
		return ;
	}
	pid_t pid = Fork();
	if (pid == 0) {
		close(fd[0][0]);
		dup2(fd[0][1], STDOUT_FILENO);
		close(fd[0][1]);

		if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, tmp[0]);
			tmp[pipe_i] = NULL;
			externFunction(filename, tmp, environ);
		} // background 추가
		exit(1);
	} else if (pid > 0) {
		close(fd[0][1]);
		*index += pipe_i + 1;
	}
}

int	midPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

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
		if (pid == 0) { // 자식
			close(fd[i][0]);
			dup2(fd[i - 1][0], STDIN_FILENO);
			close(fd[i - 1][0]);
			dup2(fd[i][1], STDOUT_FILENO);
			close(fd[i][1]);
			if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
				char *filename = (char *)malloc(sizeof(char) * MAXLINE);
				strcpy(filename, tmp[0]);
				tmp[pipe_i] = NULL;
				externFunction(filename, tmp, environ);
			} // background 추가
			exit(1);
		}
		else if (pid > 0) {
			close(fd[i - 1][0]);
			close(fd[i][1]);
			*index += pipe_i + 1;
		} else
			return (-1);
	}
	return (i);
}

int	lastPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i) {

	int pipe_i = 0;
	char **tmp = argv + (*index);
	for (; (tmp[pipe_i] && tmp[pipe_i][0] != '|'); pipe_i++);

	pid_t pid = Fork();
	int status = 0;

	if (pid == 0) {	// 자식
		dup2(fd[i][0], STDIN_FILENO);
		close(fd[i][0]);
		if (!builtinCommand(explamation, cmdline, tmp, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, tmp[0]);
			tmp[pipe_i] = NULL;
			externFunction(filename, tmp, environ);
		} // background 추가
		exit(1);
	} else if (pid > 0) {
		close(fd[i][0]);
		i = -1;
		while (++i < pipe_count + 1) {
			int tmp;
			if (pid == wait(&tmp))
				status = tmp;
		}
	}
	return (status);
}
