#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include "csapp.h"

#define MAXARGS 128
#define MAXLINE 8192

/* Function prototypes */
int parseline(char *cmdline, char *buf, char **argv, int *pipe_count);
void eval(char *cmdline, FILE *fp_history, int *history_count);
int builtinCommand(char *cmdline, char **argv, FILE *fp_history, int *history_count);
void externFunction(char *filename, char **argv, char **environ);
void noPipe(int pipe_count, int bg, char *cmdline, char **argv, FILE *fp_history, int *history_count);
void firstPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
int midPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd);
int lastPipe(int pipe_count, int bg, char *cmdline, char **argv, int *index, FILE *fp_history, int *history_count, int **fd, int i);

#endif