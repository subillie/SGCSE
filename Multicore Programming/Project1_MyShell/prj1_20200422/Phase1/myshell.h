#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include <readline/readline.h>
#include <readline/history.h>
#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

/* External variables */
extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *cmdline, char *buf, char **argv);

#endif
