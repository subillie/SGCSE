#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

sigset_t mask_all, mask_one, prev_one;
volatile int signal_flag;
volatile sig_atomic_t pid;

/* Function prototypes - signal.c */
void initSignal();
/* Function prototypes - evaluator.c */
void eval(char *cmdline, FILE *fp_history, int *history_count);
/* Function prototypes - parser.c */
int parseline(char *cmdline, char *buf, char **argv);
/* Function prototypes - executer.c */
int addHistory(char *cmdline, char *buf, char **argv, FILE *fp_history, int *history_count);
int builtinCommand(int explamation, char *cmdline, char **argv, FILE *fp_history, int *history_count);
void externFunction(char *filename, char **argv, char **environ);

#endif
