#include "myshell.h"

// addJob - Add a job to the job entry
static void addJob(pid_t pid, int state, char *cmdline) {

	job_entry_t *job = (job_entry_t *) malloc(sizeof(job_entry_t));
	job->pid = pid;
	job->state = state;
	strcpy(job->cmdline, cmdline);
	job->next = NULL;

	if (job->state == BG) {			// BG gets job id
		job_entry_t *tmp;
		int max_jid = 0;
		for (tmp = job_front; tmp != NULL; tmp = tmp->next)
			if (tmp->jid > max_jid)
				max_jid = tmp->jid;
		job->jid = max_jid + 1;
	} else if (job->state == FG)	// FG doesn't get job id
		job->jid = -1;

	// If empty
	if (job_front == NULL) {
		job_front = job_last = job;
		job_count = 1;
	}
	// Not empty
	else {
		job_last->next = job;
		job_last = job;
		job_count++;
	}
}

// updateJob - Update job state
static void updateJob(pid_t pid, int state) {

	job_entry_t *job;
	for (job = job_front; job; job = job->next) {
		if (job->pid == pid) {
			job->state = state;
			if (state == DONE)
				job_count--;
			break;
		}
	}
}

// getJob - Get job entry by pid
job_entry_t *getJob(int pid) {

	job_entry_t *job;
	for (job = job_front; job; job = job->next)
		if (job->pid == pid)
			return job;
	return NULL;
}

// deleteJob - Delete a job from the job entry
void deleteJob(int jid) {

	job_entry_t *job = job_front, *prev = job_front;
	while (job && job->jid != jid) {
		prev = job;
		job = job->next;
	}

	// no such job
	if (!job)
		return;

	// found the job
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
	job_count--;
}

void printJob() {

	job_entry_t *job;
	for (job = job_front; job != NULL; job = job->next) {
		// if (job->pid != 0) {
			if (job->state == BG) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("]  RUNNING      ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts(" &\n");
			} else if (job->state == FG) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("]  RUNNING      ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == ST) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("]  STOPPED      ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == DONE) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("]  DONE         ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			} else if (job->state == TT) {
				Sio_puts("[");
				Sio_putl((long)job->jid);
				Sio_puts("]  TERMINATED   ");
				Sio_putl((long)job->pid);
				Sio_puts(" ");
				Sio_puts(job->cmdline);
				Sio_puts("\n");
			}
		}
	// }

	job_entry_t *job_next;
	for (job = job_front; job; job = job_next) {
		job_next = job->next;
		if (job->state == DONE || job->state == TT)
			deleteJob(job->jid);
	}
}
