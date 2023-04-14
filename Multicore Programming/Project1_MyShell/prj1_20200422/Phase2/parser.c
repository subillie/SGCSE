#include "myshell.h"

/* Function prototypes */
static void render_line(char *cmdline, char *buf, int *pipe_count);

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *cmdline, char *buf, char **argv, int *pipe_count) { //TODO quote 처리

	char *delim;	// Points to opening_q space delimiter
	int argc = 0;	// Number of args
	int bg;			// Background job?

	strcpy(buf, cmdline);
	render_line(cmdline, buf, pipe_count);

	buf[strlen(buf)-1] = ' ';		// Replace trailing '\n' with space
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

	if (!argv[0][0]) {
		for (argc = 0; argv[argc + 1]; argc++)
			argv[argc] = argv[argc + 1];
		argv[argc] = NULL;
	}

	// Ignore blank line
	if (argc == 0)
		return 1;

	// Should the job run in the background?
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;
	return bg;
}

/* Render cmdline and copy to buf */
static void render_line(char *cmdline, char *buf, int *pipe_count) {

	// Put space besides | and &
	int i_b = 0, i_c = 0;
	while (cmdline[i_c] && i_b < MAXLINE - 1) {
		if (cmdline[i_c] == '|') {
			buf[i_b++] = ' ';
			buf[i_b++] = cmdline[i_c++];
			buf[i_b++] = ' ';
			(*pipe_count)++;
		} else if (cmdline[i_c] == '&') {
			buf[i_b++] = ' ';
			buf[i_b++] = cmdline[i_c++];
			buf[i_b++] = ' ';
		} else
			buf[i_b++] = cmdline[i_c++];
	}

	// Remove trailing newline
	cmdline[strlen(cmdline) - 1] = '\0';

	// Replace tab with space
	i_b = -1;
	while (buf[++i_b])
		if (buf[i_b] == '\t')
			buf[i_b] = ' ';
}
/* $end parseline */
