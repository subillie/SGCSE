#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 8192

void simplify(char *cmdline, char *buf) {

	int i_b, i_c = -1;

	/* Replace tab with space */
	while (cmdline[++i_c])
		if (cmdline[i_c] == '\t')
			cmdline[i_c] = ' ';

	/* Put space besides | and & */
	i_b = 0, i_c = 0;
	while (cmdline[i_c] && i_b < MAXLINE - 1) {
		if (cmdline[i_c] == '|' || cmdline[i_c] == '&') {
			buf[i_b++] = ' ';
			buf[i_b++] = cmdline[i_c++];
			buf[i_b++] = ' ';
		} else
			buf[i_b++] = cmdline[i_c++];
	}
	buf[i_b] = '\0';
}

char **parseline(char *cmdline, char *buf, char **argv) {

	char *delim;	/* Points to first space delimiter */
	int argc = 0;	/* Number of args */

	cmdline[strlen(cmdline)-1] = ' ';		/* Replace trailing '\n' with space */
	while (*cmdline && (*cmdline == ' '))	/* Ignore leading spaces */
		cmdline++;
	if (*cmdline == '\0')	/* Ignore blank line */
		return (NULL);
	simplify(cmdline, buf);

	/* Build the argv list */
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' '))	/* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	return argv;
}

int main()
{
	char cmdline[] = "   a  &  c  d  e  f& g ||  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z   ";
	char buf[MAXLINE];
	char **argv = malloc((strlen(cmdline) + 1) * sizeof(char *));
	argv = parseline(cmdline, buf, argv);
	for (int i = 0; argv[i] != NULL; i++)
		printf("%s\n", argv[i]);
	free(argv);
	return 0;
}