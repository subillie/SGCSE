#ifndef __MYSHELL_H__
#define __MYSHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXARGS 128
#define MAXLINE 8192

/* External variables */
extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

/************************** 
 * Error-handling functions
 **************************/

/* Unix-style error */
void unix_error(char *msg) {

	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(size_t size) {

	void *p;

	if ((p  = malloc(size)) == NULL)
		unix_error("Malloc error");
	return p;
}

void Free(void *ptr) {

	free(ptr);
}

/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

pid_t Fork(void) {

	pid_t pid;

	if ((pid = fork()) < 0)
		unix_error("Fork error");
	return pid;
}

void Execve(const char *filename, char *const argv[], char *const envp[]) {

	if (execve(filename, argv, envp) < 0)
		unix_error("Execve error");
}

pid_t Wait(int *status) {

	pid_t pid;

	if ((pid  = wait(status)) < 0)
		unix_error("Wait error");
	return pid;
}

pid_t Waitpid(pid_t pid, int *iptr, int options) {

	pid_t retpid;

	if ((retpid  = waitpid(pid, iptr, options)) < 0) 
		unix_error("Waitpid error");
	return(retpid);
}

void Kill(pid_t pid, int signum) {

	int rc;

	if ((rc = kill(pid, signum)) < 0)
		unix_error("Kill error");
}

void Pause() {

	(void)pause();
	return;
}

unsigned int Sleep(unsigned int secs) {

	return sleep(secs);
}

unsigned int Alarm(unsigned int seconds) {

	return alarm(seconds);
}
 
void Setpgid(pid_t pid, pid_t pgid) {

	int rc;

	if ((rc = setpgid(pid, pgid)) < 0)
		unix_error("Setpgid error");
	return;
}

pid_t Getpgrp(void) {

	return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions 
 ***********************************/
typedef void handler_t(int);

handler_t *Signal(int signum, handler_t *handler) {

	struct sigaction action, old_action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
	action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

	if (sigaction(signum, &action, &old_action) < 0)
		unix_error("Signal error");
	return (old_action.sa_handler);
}

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {

	if (sigprocmask(how, set, oldset) < 0)
		unix_error("Sigprocmask error");
	return;
}

void Sigemptyset(sigset_t *set) {

	if (sigemptyset(set) < 0)
		unix_error("Sigemptyset error");
	return;
}

void Sigfillset(sigset_t *set) {

	if (sigfillset(set) < 0)
		unix_error("Sigfillset error");
	return;
}

void Sigaddset(sigset_t *set, int signum) {

	if (sigaddset(set, signum) < 0)
		unix_error("Sigaddset error");
	return;
}

void Sigdelset(sigset_t *set, int signum) {

	if (sigdelset(set, signum) < 0)
		unix_error("Sigdelset error");
	return;
}

int Sigismember(const sigset_t *set, int signum) {

	int rc;

	if ((rc = sigismember(set, signum)) < 0)
		unix_error("Sigismember error");
	return rc;
}

int Sigsuspend(const sigset_t *set) {

	int rc = sigsuspend(set); /* always returns -1 */

	if (errno != EINTR)
		unix_error("Sigsuspend error");
	return rc;
}

#endif
