/* 
* server_sp.c - utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void *thread(void *vargp, int connfd) {

	char buf[MAXLINE];
	rio_t rio;
	int n;

	Pthread_detach(pthread_self());
	while (1) {
		int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
		Rio_readinitb(&rio, connfd);
		while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
			if (strcmp(buf, "exit\n") == 0) break; // Exit the loop for "exit" command
			execute_command(buf, connfd);
		}
		Close(connfd);
	}
}

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {

	sp->buf = Calloc(n, sizeof(int));
	sp->n = n;					/* Buffer holds max of n items */
	sp->front = sp->rear = 0;	/* Empty buffer iff front == rear */
	Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking */
	Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */
	Sem_init(&sp->items, 0, 0); /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
void sbuf_deinit(sbuf_t *sp) {

	Free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item) {

	P(&sp->slots);
	P(&sp->mutex);
	sp->buf[(++sp->rear) % (sp->n)] = item;
	V(&sp->mutex);
	V(&sp->items);
}

int sbuf_remove(sbuf_t *sp) {

	int item;
	P(&sp->items);
	P(&sp->mutex);
	item = sp->buf[(++sp->front) % (sp->n)];
	V(&sp->mutex);
	V(&sp->slots);
	return item;
}