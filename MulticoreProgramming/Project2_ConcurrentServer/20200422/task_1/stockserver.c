/* 
* stockserver.c - A concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void sigint_handler(int sig) {

	upload_file();
	free_tree(root);
	exit(0);
}

/* $begin stockserver main */
int main(int argc, char **argv) {

	int listenfd, connfd, buflen;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr; /* Enough space for any address */ //line:netp:echoserveri:sockaddrstorage
	char client_hostname[MAXLINE], client_port[MAXLINE], buf[MAXLINE];
	static pool_t pool;
	rio_t rio;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	Signal(SIGINT, sigint_handler);

	listenfd = Open_listenfd(argv[1]);
	init_pool(listenfd, &pool);
	while (1) {
		/* Wait for listening/connected descriptor(s) to become ready */
		pool.ready_set = pool.read_set;
		pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

		for (int i = 0; (i < pool.maxi) && (pool.nready > 0); i++) {
			connfd = pool.clientfd[i];
			rio = pool.clientrio[i];

			/* If the descriptor is ready, execute the command */
			if ((connfd > 0) && (FD_ISSET(connfd, &pool.ready_set))) { /* Skip inactive connections */
				pool.nready--;
				if ((buflen = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
					printf("Server received %d bytes\n", buflen);
					execute_command(i, connfd, buflen, buf, &pool);
				}
				/* EOF detected, remove descriptor from pool */
				else {
					Close(connfd);
					FD_CLR(connfd, &pool.read_set);
					pool.clientfd[i] = -1;
					upload_file();
				}
			}
			/* If listening descriptor ready, add new client to pool */
			else if ((FD_ISSET(connfd, &pool.ready_set))) {
				clientlen = sizeof(struct sockaddr_storage);
				connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
				Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
				printf("Connected to (%s, %s)\n", client_hostname, client_port);
				add_client(connfd, &pool);
			}
		}
	}
	exit(0);
}
/* $end stockserver main */
