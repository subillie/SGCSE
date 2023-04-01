#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *simplify(char *cmdline, char *buf) {

	int i_b, i_c = -1;

	/* Replace tab with space */
	while (cmdline[++i_c])
		if (cmdline[i_c] == '\t')
			cmdline[i_c] = ' ';

	/* Put space besides | and & */
	for (i_b = 0, i_c = 0; cmdline[i_c]; i_c++) {
		if (cmdline[i_c] == '|' || \
			(cmdline[i_c - 1] != '|' && cmdline[i_c] == '&')) {
			buf[i_b++] = ' ';
			buf[i_b++] = cmdline[i_c];
			buf[i_b++] = ' ';
		} else
			buf[i_b++] = cmdline[i_c];
	}
	buf[i_b] = '\0';
	return (buf);
}

char **parseline(char *cmdline, char *buf, char **argv) {

	char *delim;	/* Points to first space delimiter */
	int argc;		/* Number of args */
	int bg;			/* Background job? */

	cmdline[strlen(cmdline)-1] = ' ';		/* Replace trailing '\n' with space */
	while (*cmdline && (*cmdline == ' '))	/* Ignore leading spaces */
		cmdline++;
	simplify(cmdline, buf);

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		printf("delim: %s, buf: %s\n", delim, buf);
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))	/* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	return (argv);
}

int main()
{
	char cmdline[] = "   a  &  c  d  e  f& g ||  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z   ";
	char *buf = malloc(strlen(cmdline) + 1);
	char **argv = malloc(100 * sizeof(char *));
	argv = parseline(simplify(cmdline, buf), buf, argv);
	for (int i = 0; argv[i] != NULL; i++)
		printf("%s\n", argv[i]);
	free(buf);  // free dynamically allocated memory
	free(argv); // free dynamically allocated memory
	return 0;
}
