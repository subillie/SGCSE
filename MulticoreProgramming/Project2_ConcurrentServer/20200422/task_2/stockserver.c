/* 
* stockserver.c - A concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

static void sigint_handler(int sig) {

	update_textfile();
	sbuf_deinit(&sbuf);
	free_tree(root);
	exit(0);
}

/* $begin stockserver main */
int main(int argc, char **argv) {

	int i, listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	pthread_t tid;
	char client_hostname[MAXLINE], client_port[MAXLINE];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	root = NULL;
	Signal(SIGINT, sigint_handler);
	Sem_init(&file_mutex, 0, 1);
	listenfd = Open_listenfd(argv[1]);
	sbuf_init(&sbuf, 1024); /* Give the max num of slots to the second param. */
	init_items();
	for (i = 0; i < sbuf.n; i++) /* Create worker threads */
		Pthread_create(&tid, NULL, thread, NULL);

	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
		printf("Connected to (%s, %s)\n", client_hostname, client_port);
		sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
	}
//	for (i = 0; i < sbuf.n; i++)
//		Pthread_join();
}
/* $end stockserver main */
