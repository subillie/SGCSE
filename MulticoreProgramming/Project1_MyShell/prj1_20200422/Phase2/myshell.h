#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

/* Function prototypes - evaluator.c */
void eval(char *cmdline, FILE *fp_history, int *history_count);
/* Function prototypes - parser.c */
int parseline(char *cmdline, char *buf, char **argv, int *pipe_count);
/* Function prototypes - executer.c */
int addHistory(char *cmdline, char *buf, char **argv, FILE *fp_history, int *history_count);
int builtinCommand(int explamation, char *cmdline, char **argv, FILE *fp_history, int *history_count);
void externFunction(char *filename, char **argv, char **environ);
/* Function prototypes - pipe.c */
void firstPipe(int explamation, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
int midPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
int lastPipe(int explamation, int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i);

#endif