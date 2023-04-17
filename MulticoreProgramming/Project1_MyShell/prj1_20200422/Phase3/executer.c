#include "myshell.h"

int addHistory(char *cmdline, char *buf, char **argv, FILE *fp_history, int *history_count) {

	int explamation = -1;

	// Check if there is !! or !#
	for (int i = 0; argv[i]; i++) {
		for (int j = 1; argv[i][j]; j++) {
			// If there is !!, put newline besides the command
			if (argv[i][j - 1] == '!' && argv[i][j] == '!') {
				explamation = 1;
				break;
			}
			// If there is !#, put newline besides the command
			else if (argv[i][j - 1] == '!' && isdigit(argv[i][j])) {	// If there is !#
				explamation = 2;
				break;
			}
		}
		// If there is !! or !#, break
		if (explamation > 0)
			break;
	}

	if ((strlen(buf) > 0) && (explamation == -1)) {
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

	return explamation;
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
int builtinCommand(int explamation, char *cmdline, char **argv, FILE *fp_history, int *history_count) {

	if (!strcmp(argv[0], "exit"))			// exit command
		exit(0);

	if (!strcmp(argv[0], "&"))				// Ignore singleton &
		return 1;

	if (strcmp(argv[0], "cd") == 0) {		// cd command
		if (argv[1] == NULL)
			chdir(getenv("HOME"));
		else if (chdir(argv[1]) < 0)
			fprintf(stderr, "-bash: cd: %s: no such file or directory\n", argv[1]);
		return 1;
	}

	if (strcmp(argv[0], "history") == 0) {	// history command
		fseek(fp_history, 0L, SEEK_SET);
		char line[MAXLINE];
		while (fgets(line, MAXLINE, fp_history) != NULL)
			printf("%s", line);
		return 1;
	}

	if (explamation == 1) {					// !! command
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

	if (explamation == 2) {					// !# command
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

	if (strcmp(argv[0], "jobs") == 0) {		// jobs command
		printJob();
		return 1;
	}

	pid_t pid;
	job_entry_t *job;
	if (strcmp(argv[0], "bg") == 0) {		// bg command
		job_entry_t *latest_stjob = NULL;

		// If there is no argument
		if (!argv[1]) {
			for (job = job_front; job; job = job->next)
				if (job->state == ST)
					latest_stjob = job;
			if (latest_stjob) {	// Resume the latest stopped job
				latest_stjob->state = BG;
				kill(latest_stjob->pid, SIGCONT);
				return 1;
			} else {			// Nothing specified
				Sio_puts("-bash: bg: no such job or already in background\n");
				return 1;
			}
		}

		// If there is an argument
		int jid;
		if (argv[1][0] == '%')
			jid = atoi(&argv[1][1]);
		else
			jid = atoi(argv[1]);
		for (job = job_front; job; job = job->next) {
			if (job->jid == jid && job->state == ST) {
				pid = job->pid;
				if (job->pid == 0)
					break;
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] ");
				Sio_puts((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
				job->state = BG;
				kill(job->pid, SIGCONT);
				return 1;
			}
		}
		Sio_puts("-bash: bg: no such job or already in background\n");
		return 1;
	}

	if (strcmp(argv[0], "fg") == 0) {		// fg command

		return 1;
	}

	if (strcmp(argv[0], "kill") == 0) {		// kill command

		return 1;
	}

	return 0;	// Not a builtin command
}
