#include "myshell.h"

/* Extern values */
int signal_flag = 0;
int explamation_flag = -2;

/* Function prototypes */
static void handler();
static void addHistory(char *cmdline, t_history **history);
static void externFunction(char *filename, char *argv[], char *environ[]);
static int builtinCommand(char **argv, t_history **history);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, t_history **history) {

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
	if (argv[0] == NULL) // Ignore empty lines
		return;
	for (int i = 0; argv[i]; i++) { // Check if there is !! or !#
		for (int j = 1; argv[i][j]; j++) {
			if (argv[i][j - 1] == '!' && argv[i][j] == '!') {
				explamation_flag = -1;
				break;
			}
			else if (argv[i][j - 1] == '!' && isdigit(argv[i][j])) {
				explamation_flag = i;
				break;
			}
		}
		if (explamation_flag)
			break;
	}
	if ((strlen(buf) > 0) && (explamation_flag == 0)) {
		add_history(cmdline);
		addHistory(cmdline, history);
	}

	// quit -> exit(0), & -> ignore, other -> run
	if (!builtinCommand(argv, history)) {
		Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
		char *filename = argv[0];
		// Child runs user job
		if ((pid = Fork()) == 0) {
			Sigprocmask(SIG_SETMASK, &prev_one, NULL);
			// setpgid(0, 0);
			externFunction(filename, argv, environ);
		}
		Sigprocmask(SIG_BLOCK, &mask_all, NULL);
		Sigprocmask(SIG_SETMASK, &prev_one, NULL);
	}
		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
			// if (waitpid(pid, &status, 0) < 0)
				// unix_error("waitfg: waitpid error");
		}
		else	// when there is background process
			printf("%d %s", pid, cmdline);
	return;
}

/* handler - Handle signal */
static void handler() {

	int olderrno = errno;
	int status;
	pid_t pid;
	sigset_t mask_all, prev;

	Sigfillset(&mask_all);
	// Wait for a child process to stop or terminate
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

static void addHistory(char *cmdline, t_history **history) {

	t_history *new = (t_history *)malloc(sizeof(t_history));
	if (new == NULL)
		unix_error("malloc error\n");
	cmdline[strlen(cmdline) - 1] = '\0';
	new->cmd = (char *)malloc(sizeof(char) * strlen(cmdline));
	if (new->cmd == NULL)
		unix_error("malloc error\n");
	strcpy(new->cmd, cmdline);
	new->next = NULL;

	if (*history == NULL)
		*history = new;
	else {
		t_history *cur = *history;
		while (cur->next)
			cur = cur->next;
		if (strcmp(cur->cmd, cmdline))
			cur->next = new;
		else {	// If the same command is entered, delete the new node
			free(new->cmd);
			free(new);
		}
	}
}

static void externFunction(char *filename, char *argv[], char *environ[]) {

	if (execve(filename, argv, environ) < 0) {	// e.g. /bin/ls ls -al &
		char *path = getenv("PATH");
		for (int i = 0; path[i]; i++) {
			if (path[i] == ':') {
				path[i] = '\0';
				strcpy(filename, path);
				strlcat(filename, "/", strlen(filename) + 2);
				strlcat(filename, argv[0], strlen(filename) + strlen(argv[0]) + 1);
				printf("path? %s\n", filename);
				if (execve(filename, argv, environ) < 0) {
					path[i] = ':';
					path += i + 1;
					i = 0;
				} else
					break;
			}
		}
		free(path); // TODO: free or not?
		free(filename);
		fprintf(stderr, "CSE4100: %s: Command not found.\n", argv[0]);
		exit (1);
	}
}

/* If opening_q arg is a builtin command, run it and return true */
static int builtinCommand(char **argv, t_history **history) {

	if (!strcmp(argv[0], "exit"))		// exit command
		exit(0);
	if (!strcmp(argv[0], "&"))			// Ignore singleton &
		return 1;

	if (!strcmp(argv[0], "cd")) {		// cd command
		if (argv[1] == NULL)
			chdir(getenv("HOME"));
		else if (chdir(argv[1]) < 0) {
			if (!argv[2])
				fprintf(stderr, "CSE4100: cd: no such file or directory:%s\n", argv[1]); // TODO ㄷㅏ stderr로 바바꿔꿔
			else {
				fprintf(stderr, "CSE4100: cd : string not in pwd:");
				for (int i = 2; argv[i]; i++)
					printf(" %s", argv[i]);
				printf("\n");
			}
		}
		return 1;
	}

	if (!strcmp(argv[0], "mkdir")) {	// mkdir command
		if (argv[1] == NULL)
			fprintf(stderr, "usage: mkdir [-pv] [-m mode] directory ...\n");
		else if (mkdir(argv[1], 0755) < 0)
			fprintf(stderr, "CSE4100: mkdir: %s: File exists\n", argv[1]);
		return 1;
	}

	if (!strcmp(argv[0], "rmdir")) {	// rmdir command
		if (argv[1] == NULL)
			fprintf(stderr, "usage: rmdir [-p] directory ...\n");
		else if (rmdir(argv[1]) < 0)
			fprintf(stderr, "CSE4100: rmdir: %s: No such file or directory\n", argv[1]);
		return 1;
	}

	if (!strcmp(argv[0], "cat")) {		// cat command
		if (argv[1] == NULL)
			fprintf(stderr, "usage: cat: missing operand\n");
		else {
			int fd = open(argv[1], O_RDONLY);
			if (fd < 0) {
				printf("CSE4100: cat: %s: No such file or directory\n", argv[1]);
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

	if (!strcmp(argv[0], "history")) {	// history command
		if (*history == NULL)
			return 1;
		else {
			t_history *cur = *history;
			for (int i = 1; cur; i++) {
				printf("%-4d%s\n", i, cur->cmd);
				cur = cur->next;
			}
			return 1;
		}
	}

	if (explamation_flag == -1) {		// !! command
		if (*history == NULL) {
			explamation_flag = 0;
			return 1;
		}
		else {
			t_history *cur = *history;
			while (cur->next)
				cur = cur->next;
			for (int i = 0; argv[i]; i++) {
				char *token = strtok(argv[i], "!!"); // TODO !만 인식되는지 !!가 인식되는지
				if (token == NULL)
					printf("%s", cur->cmd);
				while (token != NULL) {
					char *prev_token = token;
					token = strtok(NULL, "!!");
					if (token == NULL)
						printf("%s", cur->cmd);
					printf("%s", prev_token);
				}
			}
			printf("\n");
			eval(cur->cmd, history);
			explamation_flag = -2;
			return 1;
		}
	}

	if (explamation_flag > -1) {		// !# command
		int start = 0;
		while ((argv[explamation_flag][start] == '!') && isdigit(argv[explamation_flag][start + 1]))
			start++;
		int num = atoi(argv[explamation_flag][start]부터 끝까지); //TODO
		int count = 1;
		t_history *cur = *history;
		while (cur->next && count < num) {
			count++;
			cur = cur->next;
		}
		if (count == num && cur) {
			for (int i = 0; argv[i]; i++) {
				argv[explamation_flag] -= 1; //TODO
				char *token = strtok(argv[i], argv[explamation_flag]);
				if (token == NULL)
					printf("%s", cur->cmd);
				while (token != NULL) {
					char *prev_token = token;
					token = strtok(NULL, (argv[explamation_flag]));
					if (token == NULL)
						printf("%s", cur->cmd);
					printf("%s", prev_token);
				}
			}
			printf("\n");
		}
		else
			fprintf(stderr, "CSE4100: %s: event not found\n", argv[0]);
		explamation_flag = -2;
		return 1;
	}

	return 0;							// Not a builtin command
}
/* $end eval */
