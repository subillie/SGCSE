#include "myshell.h"

void	firstPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

	if (pipe(fd[0]) == -1) {
		unix_error("Pipe error");
		return ;
	}
	pid_t pid = Fork();
	printf("=============================\n");
	for (char **tmp = argv; *tmp; tmp++)
		printf("fst: %s\n", *tmp);
	printf("=============================\n");
	if (pid == 0) {
		close(fd[0][0]);
		dup2(fd[0][1], STDOUT_FILENO);
		close(fd[0][1]);
		if (!builtinCommand(cmdline, argv, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, argv[0]);
			externFunction(filename, argv, environ);
		} // background 추가
		exit(1);
	} else if (pid > 0) {
		close(fd[0][1]);
		*index += 2;
	}
}

static void	run_child(int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count, int **fd, int i) {

	close(fd[i][0]);
	dup2(fd[i - 1][0], STDIN_FILENO);
	close(fd[i - 1][0]);
	dup2(fd[i][1], STDOUT_FILENO);
	close(fd[i][1]);
	if (!builtinCommand(cmdline, argv, fp_history, history_count)) {
		char *filename = (char *)malloc(sizeof(char) * MAXLINE);
		strcpy(filename, argv[0]);
		externFunction(filename, argv, environ);
	} // background 추가
	exit(1);
}

int	midPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd) {

	printf("=============================\n");
	for (char **tmp = (argv + (*index)); *tmp; tmp++)
		printf("mid: %s\n", *tmp);
	printf("=============================\n");
	int i = 0;
	while (++i < pipe_count) {
		if (pipe(fd[i]) == -1) {
			unix_error("Pipe error");
			return (-1);
		}
		pid_t pid = Fork();
		if (pid == 0) // 자식
			run_child(pipe_count, bg, cmdline, (argv + (*index)), fp_history, history_count, fd, i);
		else if (pid > 0) {
			close(fd[i - 1][0]);
			close(fd[i][1]);
			*index += 2;
		} else
			return (-1);
	}
	return (i);
}

int	lastPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i) {

	pid_t pid = Fork();
	int status = 0;

	printf("=============================\n");
	for (char **tmp = (argv + (*index)); *tmp; tmp++)
		printf("lst: %s\n", *tmp);
	printf("=============================\n");
	if (pid == 0) {	// 자식
		dup2(fd[i][0], STDIN_FILENO);
		close(fd[i][0]);
		if (!builtinCommand(cmdline, argv, fp_history, history_count)) {
			char *filename = (char *)malloc(sizeof(char) * MAXLINE);
			strcpy(filename, argv[0]);
			externFunction(filename, argv, environ);
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
