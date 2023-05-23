/* 
* stockserver.c - A concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

sbuf_t sbuf; /* Shared buffer of connected descriptors */

static void sigint_handler(int sig) {

	update_textfile();
	free_tree(root);
	exit(0);
}

/* $begin stockserver main */
int main(int argc, char **argv) {

	int i, listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	pthread_t tid;

	Signal(SIGINT, sigint_handler);
	listenfd = Open_listenfd(argv[1]);
	sbuf_init(&sbuf, SBUFSIZE);

	for (i = 0; i < NTHREADS; i++) /* Create worker threads */
		Pthread_create(&tid, NULL, thread, NULL);

	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
	}
}
/* $end stockserver main */
