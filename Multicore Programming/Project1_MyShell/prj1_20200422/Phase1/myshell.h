#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include <readline/history.h>
#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

/* Function prototypes */
void eval(char *cmdline, FILE *fp_history);
int parseline(char *cmdline, char *buf, char **argv);

#endif
