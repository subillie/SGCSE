#include "myshell.h"

/* Function prototypes */
static void handlerSIGINT(int sig);
static void handlerSIGTSTP(int sig);
static void handlerSIGCHLD(int sig);

void initSignal() {

	// Init mask_all
	Sigfillset(&mask_all);

	// Init mask_one
	Sigemptyset(&mask_one);
	Sigaddset(&mask_one, SIGCHLD);

	// void (*signal(int signum, void (*handler)(int)))(int)
	Signal(SIGCHLD, handlerSIGCHLD);
	Signal(SIGINT, handlerSIGINT);	// SIGINT - interrupt from keyboard
	Signal(SIGTSTP, handlerSIGTSTP);	// SIGTSTP - stop execution
//	Signal(SIGQUIT, SIG_IGN);			// SIGQUIT - quit from keyboard
	Signal(SIGTTIN, SIG_IGN);		// SIGTTIN - background process read attempt
	Signal(SIGTTOU, SIG_IGN);		// SIGTTOU - background process의 write attempt
}

static void handlerSIGINT(int sig) {

	if (pid != 0)
		kill(-pid, SIGINT);
//	printf("\nCSE4100-MP-P1> ");
	fflush(stdout);
	signal_flag = 2;
	return;
}

static void handlerSIGTSTP(int sig) {

	if (pid != 0)
		kill(-pid, SIGINT);
	signal_flag = 3;
	unix_error("exit");
	return;
}

static void handlerSIGCHLD(int sig) {

	int olderrno = errno;

	int status;
	sigset_t mask, prev;
	Sigfillset(&mask);

	// WNOHANG - return immediately instead of waiting, if there is no child process ready to be noticed
	// WUNTRACED - report the status of any child processes that have been stopped as well as those that have terminated
	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
		Sigprocmask(SIG_BLOCK, &mask, &prev);
		// Set signal_flag if stopped, exited, or signaled
		if (WIFSTOPPED(status) || WIFEXITED(status) || WIFSIGNALED(status))
			signal_flag = 1;
		Sigprocmask(SIG_SETMASK, &prev, NULL);
	}
	errno = olderrno;
}
