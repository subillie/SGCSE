#include "myshell.h"

/* Function prototypes */
static int render_line(char *cmdline, char *buf);

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *cmdline, char *buf, char **argv) {

	char *delim;	// Points to opening_q space delimiter
	int argc = 0;	// Number of args
	int bg;			// Background job?

	int flag = render_line(cmdline, buf);
	if (flag >= 0) {
		int i = 0;
		for (; buf[flag + i + 1]; i++)
			buf[flag + i] = buf[flag + i + 1];
		buf[flag] = '\n';
	}
	buf[strlen(buf) - 1] = ' ';		// Replace trailing '\n' with space
	for (int i = 0; (buf[i] && buf[i] == ' '); i++)	// Ignore leading spaces
		buf++;

	// Build the argv list
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))	// Ignore spaces
			buf++;
	}
	argv[argc] = NULL;

	// Ignore blank line
	if (argv == NULL || argv[0] == NULL || (argv[0][0] == '\0' && argv[1] == NULL))
		return (-1);

	// If a command follows by a blank line, ignore the blank line
	if (argv[0][0] == '\0') {
		for (argc = 0; argv[argc + 1]; argc++)
			argv[argc] = argv[argc + 1];
		argv[argc] = NULL;
	}

	// Should the job run in the background?
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;
	return bg;
}

/* Render cmdline and copy to buf */
static int render_line(char *cmdline, char *buf) {

	int i_b = -1, i_c = -1, count = 0;
	int single_quote = -1, double_quote = -1;
	int *match = (int *)Malloc(sizeof(int) * (strlen(cmdline) + 1));

	while (cmdline[++i_c]) {
		if (cmdline[i_c] == '\'' && single_quote < 0) {
			match[i_c] = single_quote;
			single_quote = i_c;
		} else if (cmdline[i_c] == '\"' && double_quote < 0) {
			match[i_c] = double_quote;
			double_quote = i_c;
		} else if (cmdline[i_c] == '\'' && single_quote >= 0) {
			match[single_quote] = single_quote;
			match[i_c] = single_quote;
			single_quote = -1;
		} else if (cmdline[i_c] == '\"' && double_quote >= 0) {
			match[double_quote] = double_quote;
			match[i_c] = double_quote;
			double_quote = -1;
		} else
			match[i_c] = -2;
	}

	i_c = -1;
	int flag = -1;
	char quote = '\0';
	while (cmdline[++i_c] && i_b < MAXLINE - 1) {
		// Put space besides | and &
		if (cmdline[i_c] == '|') {
			buf[++i_b] = ' ';
			buf[++i_b] = cmdline[i_c];
			buf[++i_b] = ' ';
			count += 2;
		} else if (cmdline[i_c] == '&') {
			buf[++i_b] = ' ';
			buf[++i_b] = cmdline[i_c];
			buf[++i_b] = ' ';
			count += 2;

		// If there exist an unmatching quote
		} else if (match[i_c] == -1) {
			flag = i_c;
			quote = cmdline[i_c];
			buf[++i_b] = cmdline[i_c];

		// If there exist matching quotes
		} else if (match[i_c] > -1) {
			count -= 1;
			quote = '\0';
		}
		// Copy cmdline to buf
		else
			buf[++i_b] = cmdline[i_c];
	}
	cmdline[strlen(cmdline) - 1] = '\0';
	free(match);

	// Replace tab with space
	i_b = -1;
	while (buf[++i_b])
		if (buf[i_b] == '\t')
			buf[i_b] = ' ';
	buf[i_c + count] = '\0';

	// If there exists quote
	int buf_len = (int)strlen(buf);
	if (quote != '\0') {
		while (1) {
			char *new_cmdline = (char *) Malloc(sizeof(char) * (MAXLINE + 1));
			printf("> ");
			fgets(new_cmdline, MAXLINE, stdin);
			if (feof(stdin))
				exit(0);

			// Break if the input is the matching quote
			if (new_cmdline[0] == quote && new_cmdline[1] == '\n')
				break;
			strcpy(buf + buf_len - 1, new_cmdline);
			buf_len += (int)strlen(new_cmdline);
			free(new_cmdline);
		}
	} else
		flag = -1;
	return flag;
}
/* $end parseline */
