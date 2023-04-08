#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

/* External variables */
extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

typedef struct s_history {
	char *cmd;
	struct s_history *next;
} t_history;

/* Function prototypes */
void eval(char *cmdline, t_history **history);
int parseline(char *cmdline, char *buf, char **argv);

#endif
