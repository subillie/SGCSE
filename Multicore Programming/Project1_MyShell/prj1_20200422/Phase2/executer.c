#include "myshell.h"

/* Global variables */
int signal_flag = 0;
int explamation_flag = -1;
volatile sig_atomic_t pid;

/* Function prototypes */
static void addHistory(char *cmdline, char *buf, char **argv, FILE *fp_history, int *history_count);
static void isPipe(int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, FILE *fp_history, int *history_count) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	int pipe_count = 0;		// Check if there is pipe
	//pid_t pid;				// Process id

	// Parse command line
	bg = parseline(cmdline, buf, argv, &pipe_count);
	if (argv[0] == NULL)			// Ignore empty lines
		return;
	addHistory(cmdline, buf, argv, fp_history, history_count);

	// Execute command line
	if (pipe_count == 0)
		noPipe(pipe_count, bg, cmdline, argv, fp_history, history_count);
	else {
		int stdin_dup = dup(STDIN_FILENO);
		int stdout_dup = dup(STDOUT_FILENO);
		isPipe(pipe_count, bg, cmdline, argv, fp_history, history_count);
		dup2(stdin_dup, STDIN_FILENO);
		dup2(stdout_dup, STDOUT_FILENO);
		close(stdin_dup);
		close(stdout_dup);
	}

	return;
}

static void addHistory(char *cmdline, char *buf, char **argv, FILE *fp_history, int *history_count) {

	explamation_flag = -1;
	// Check if there is !! or !#
	for (int i = 0; argv[i]; i++) {
		for (int j = 1; argv[i][j]; j++) {
			// If there is !!, put newline besides the command
			if (argv[i][j - 1] == '!' && argv[i][j] == '!') {
				explamation_flag = 1;
				break;
			}
			// If there is !#, put newline besides the command
			else if (argv[i][j - 1] == '!' && isdigit(argv[i][j])) {	// If there is !#
				explamation_flag = 2;
				break;
			}
		}
		// If there is !! or !#, break
		if (explamation_flag > 0)
			break;
	}

	if ((strlen(buf) > 0) && (explamation_flag == -1)) {
		fseek(fp_history, 0L, SEEK_SET);
		char line[MAXLINE];
		while (fgets(line, MAXLINE, fp_history) != NULL);			// Get the last line of history
		int count = 0;
		for (int tmp = *history_count; tmp > 0; tmp /= 10)
			count++;
		strcat(cmdline, "\n");
		if (strcmp(line + count + 3, cmdline) != 0 && argv[0][0]) {	// Add history
			(*history_count)++;
			fprintf(fp_history, "%d   %s", *history_count, cmdline);
			fflush(fp_history);
		}
	}
}

void noPipe(int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	if (!builtinCommand(cmdline, argv, fp_history, history_count)) {
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
		if (pipe_count == 0) {
			int status;	//TODO
			wait(&status);
		}
	}
}

static void isPipe(int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	int **fd = Malloc(sizeof(int *) * pipe_count);
	for (int i = 0; i < pipe_count; i++)
		fd[i] = Malloc(sizeof(int) * 2);

	int index = 0;
	printf("FIRST\n");
	firstPipe(pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd);
	printf("MID\n");
	int i = midPipe(pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd);
	printf("LAST\n");
	int status = lastPipe(pipe_count, bg, cmdline, argv, &index, fp_history, history_count, fd, i);
	printf("END\n");

	for (i = 0; i < pipe_count; i++)
		free(fd[i]);
	free(fd);
	// 시그널 처리
}

void externFunction(char *filename, char **argv, char **environ) {

	int execve_flag;

	if (execve(filename, argv, environ) < 0) {	// e.g. /bin/ls ls -al &
		char *path = getenv("PATH");
		for (int i = 0; path[i]; i++) {
			if (path[i] == ':') {
				path[i] = '\0';
				strcpy(filename, path);
				strcat(filename, "/");
				strcat(filename, argv[0]);
				filename[strlen(filename)] = '\0';
				execve_flag = execve(filename, argv, environ);
				if (execve_flag < 0) {
					path[i] = ':';
					path += i + 1;
					i = 0;
				} else
					break;
			}
		}
		free(filename);
		if (execve_flag < 0 && argv[0][0])
			fprintf(stderr, "%s: Command not found.\n", argv[0]);
	}
}

/* If opening_q arg is a builtin command, run it and return true */
int builtinCommand(char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	if (!strcmp(argv[0], "exit"))		// exit command
		exit(0);

	if (!strcmp(argv[0], "&"))			// Ignore singleton &
		return 1;

	if (strcmp(argv[0], "cd") == 0) {		// cd command
		if (argv[1] == NULL)
			chdir(getenv("HOME"));
		else if (chdir(argv[1]) < 0)
			fprintf(stderr, "-bash: cd: %s: no such file or directory\n", argv[1]);
		return 1;
	}

	if (strcmp(argv[0], "mkdir") == 0) {	// mkdir command
		if (argv[1] == NULL)
			fprintf(stderr, "mkdir: missing operand\nTry 'mkdir --help' for more information.\n");
		else if (mkdir(argv[1], 0755) < 0)
			fprintf(stderr, "mkdir: cannot create directory '%s': File exitsts\n", argv[1]);
		return 1;
	}

	if (strcmp(argv[0], "rmdir") == 0) {	// rmdir command
		if (argv[1] == NULL)
			fprintf(stderr, "rmdir: missing operand\nTry 'rmdir --help' for more information.\n");
		else if (rmdir(argv[1]) < 0)
			fprintf(stderr, "rmdir: failed to remove '%s': No such file or directory\n", argv[1]);
		return 1;
	}

	if (strcmp(argv[0], "cat") == 0) {		// cat command
		if (argv[1] == NULL)
			fprintf(stderr, "usage: cat: missing operand\n");
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

	if (strcmp(argv[0], "echo") == 0) {		// echo command
		int i = 1;
		while (argv[i] != NULL) {
			printf("%s ", argv[i]);
			i++;
		}
		printf("\n");
		return 1;
	}

	if (strcmp(argv[0], "history") == 0) {	// history command
		fseek(fp_history, 0L, SEEK_SET);
		char line[MAXLINE];
		while (fgets(line, MAXLINE, fp_history) != NULL)
			printf("%s", line);
		return 1;
	}

	if (explamation_flag == 1) {		// !! command
		if (*history_count == 0)	// Return if there is no history log
			return 1;

		// Get the index of first '!'
		int exp_i, flag = 0;
		for (exp_i = 0; cmdline[exp_i]; exp_i++)
			if (cmdline[exp_i] == '!' && cmdline[exp_i + 1] == '!')
				break;

		// Get the last line of history log
		char *line = (char *)Malloc(sizeof(char) * (MAXLINE + 11));
		fseek(fp_history, 0L, SEEK_SET);
		for (int i = 0; i < *history_count; i++)
			fgets(line, MAXLINE, fp_history);

		int count = 0;
		for (int tmp = *history_count; tmp > 0; tmp /= 10)
			count++;

		// Print the command with expansion
		int i = -1;
		while (++i < exp_i)
			printf("%c", cmdline[i]);
		for (int j = 0; line[count + 3 + j] && (line[count + 3 + j] != '\n'); j++)
			printf("%c", line[count + 3 + j]);
		i += 1;
		while (cmdline[++i])
			printf("%c", cmdline[i]);
		printf("\n");

		// Evaluate the command
		eval(line + count + 3, fp_history, history_count);
		free(line);
		return 1;
	}

	if (explamation_flag == 2) {		// !# command
		// Get the index of '!'
		int exp_i, flag = 0;
		for (exp_i = 0; cmdline[exp_i]; exp_i++)
			if (cmdline[exp_i] == '!' && isdigit(cmdline[exp_i + 1]))
				break;
		char *num_str = (char *)Malloc(sizeof(char) * 11);
		int digit_count = 0;

		// Get the number from the command
		for (; isdigit(cmdline[exp_i + digit_count + 1]); digit_count++)
			num_str[digit_count] = cmdline[exp_i + digit_count + 1];
		int num = atoi(num_str);
		free(num_str);
		// Return if there is not enough history log
		if (*history_count < num)
			return 1;

		// Get the #th line from the history log
		char *line = (char *)Malloc(sizeof(char) * (MAXLINE + 11));
		fseek(fp_history, 0L, SEEK_SET);
		for (int count = 1; fgets(line, MAXLINE, fp_history) != NULL; count++)
			if (count == num)
				break;

		// Print the command with expansion
		int i = -1;
		while (++i < exp_i)
			printf("%c", cmdline[i]);
		for (int j = 0; line[digit_count + 3 + j] && (line[digit_count + 3 + j] != '\n'); j++)
			printf("%c", line[digit_count + 3 + j]);
		i += digit_count;
		while (cmdline[++i])
			printf("%c", cmdline[i]);
		printf("\n");

		// Evaluate the command
		eval(line + digit_count + 3, fp_history, history_count);
		free(line);
		return 1;
	}

	return 0;							// Not a builtin command
}
/* $end eval */
