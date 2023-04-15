#include "myshell.h"

/* Global variables */
int signal_flag = 0;
volatile sig_atomic_t pid;

/* Function prototypes */
// static void handlerSIGINT(int sig);
// static void handlerSIGTSTP(int sig);
// static void handlerSIGCHLD(int sig);

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline, FILE *fp_history, int *history_count) {

	char *argv[MAXARGS];	// Argument list execve()
	char buf[MAXLINE];		// Holds modified command line
	int bg;					// Should the job run in bg or fg?
	//pid_t pid;				// Process id
	// sigset_t mask_all, mask_one, prev_one;

	// Sigfillset(&mask_all);
	// Sigemptyset(&mask_one);
	// Sigaddset(&mask_one, SIGCHLD);
	// Signal(SIGCHLD, handlerSIGCHLD);
	// Signal(SIGINT, handlerSIGINT);
	// Signal(SIGTSTP, handlerSIGTSTP);

	// Parse command line
	bg = parseline(cmdline, buf, argv);
	if (bg == -1)	// Ignore empty line
		return;
	int explamation = addHistory(cmdline, buf, argv, fp_history, history_count);

	// Execute command line
	if (!builtinCommand(explamation, cmdline, argv, fp_history, history_count)) {
		// Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
		char *filename = (char *)malloc(sizeof(char) * MAXLINE);
		strcpy(filename, argv[0]);
		if ((pid = Fork()) == 0) {	// Child runs user job
			// Sigprocmask(SIG_SETMASK, &prev_one, NULL);
			// setpgid(0, 0);
			externFunction(filename, argv, environ);
			exit(1);
		}
		// Sigprocmask(SIG_BLOCK, &mask_all, NULL);
		// Sigprocmask(SIG_SETMASK, &prev_one, NULL);

		// Parent waits for foreground job to terminate
		if (!bg) {
			int status;
			// if (waitpid(pid, &status, 0) < 0)
			// 	unix_error("waitfg: waitpid error");
		}
		else	// when there is background process
			printf("%d %s", pid, cmdline);

		int status;	//TODO
		wait(&status);
	}

	return;
}

// static void handlerSIGINT(int sig) {

// 	if (pid != 0)
// 		kill(-pid, SIGINT);
// 	return ;
// }

// static void handlerSIGTSTP(int sig) {

// 	if (pid != 0)
// 		kill(-pid, SIGTSTP);
// 	return ;
// }

// static void handlerSIGCHLD(int sig) {
// 	int olderrno = errno;
// 	pid = waitpid(-1, NULL, 0);
// 	errno = olderrno;
// }

/* handler - Handle signal */
//static void handler() {
//
//    int olderrno = errno;
//    int status;
//    pid_t pid;
//    sigset_t mask_all, prev;
//
//    Sigfillset(&mask_all);
//    // Wait for a child process to stop or terminate
//    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED))) {
//       Sigprocmask(SIG_BLOCK, &mask_all, &prev);
//
//       // If child process is stopped or terminated, set signal_flag
//       if(WIFSTOPPED(status) || WIFEXITED(status) || WIFSIGNALED(status))
//           signal_flag = 1;
//
//       Sigprocmask(SIG_SETMASK, &prev, NULL);
//    }
//    if (errno != ECHILD)	//  ECHILD - No child processes (POSIX.1-2001).
//       unix_error("waitpid error");
//    errno = olderrno;
//}

/* $end eval */
