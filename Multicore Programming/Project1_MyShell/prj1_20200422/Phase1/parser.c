#include "myshell.h"

/* Function prototypes */
static void render_line(char *cmdline, char *buf);

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *cmdline, char *buf, char **argv) { //TODO quote 처리

	char *delim;	// Points to opening_q space delimiter
	int argc = 0;	// Number of args
	int bg;			// Background job?

	strcpy(buf, cmdline);
	render_line(cmdline, buf);

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

	// Ignore blank line
	if (argc == 0)
		return 1;

	// Should the job run in the background?
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;
	return bg;
}

/* Render cmdline and copy to buf */
static void render_line(char *cmdline, char *buf) {

	// Put space besides | and &
	int i_b = 0, i_c = 0;
	while (cmdline[i_c] && i_b < MAXLINE - 1) {
		if (cmdline[i_c] == '|' || cmdline[i_c] == '&') {
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
