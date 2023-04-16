#include "myshell.h"

static void addJob(pid_t pid, int jid, int state, char *cmdline) {

	job_entry_t *job = (job_entry_t *) malloc(sizeof(job_entry_t));
	job->pid = pid;
	job->jid = jid;
	job->state = state;
	strcpy(job->cmdline, cmdline);
	job->next = NULL;

	if (job_front == NULL) {
		job_front = job_last = job;
	} else {
		job_last->next = job;
		job_last = job;
	}
}

static void deleteJob(int jid) {

	job_entry_t *job = job_front, *prev = job_front;
	while (job && job->jid != jid) {
		prev = job;
		job = job->next;
	}

	// no such job
	if (!job)
		return;

	// found job
	if (job == job_front && job == job_last) {
		job_front = job_last = NULL;
		free(job);
	} else if (job == job_front) {
		job_front = job_front->next;
		free(job);
	} else if (job == job_last) {
		job_last = prev;
		job_last->next = NULL;
		free(job);
	} else {
		prev->next = job->next;
		free(job);
	}
}

void printJob() {

	job_entry_t *job;
	for (job = job_front; job != NULL; job = job->next) {
		if (job->pid != 0) {
			if (job->state == BG) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] RUNNING     ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("&\n");
			} else if (job->state == FG) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] RUNNING     ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == ST) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] STOPPED     ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == DONE) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] DONE        ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == TT) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("] TERMINATED  ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			}
		}
	}

	job_entry_t *job_next;
	for (job = job_front; job; job = job_next) {
		job_next = job->next;
		if (job->state == DONE || job->state == TT)
			deleteJob(job->jid);
	}
}
