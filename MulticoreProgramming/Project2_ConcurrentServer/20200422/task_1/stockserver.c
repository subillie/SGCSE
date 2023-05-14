/* 
* stockserver.c - A concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

/* $begin stockserver main */
int main(int argc, char **argv) {

	int connfd, buflen;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr; /* Enough space for any address */ //line:netp:stockserverc:sockaddrstorage
	char client_hostname[MAXLINE], client_port[MAXLINE], buf[MAXLINE];
	static pool_t pool;
	rio_t rio;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	root = NULL;
	client_head = NULL;
	client_tail = NULL;
	init_pool(argv[1], &pool);

	while (1) {
		/* Wait for listening/connected descriptor(s) to become ready */
		pool.ready_set = pool.read_set;
		pool.nready = Select(pool.listenfd + 1, &pool.ready_set, NULL, NULL, NULL);
		if (pool.nready <= 0)
			continue;

		for (client_t *ptr = client_head; ptr != NULL; ptr = ptr->next) {
			printf("got client\n");
			connfd = pool.client->fd;
			rio = pool.client->rio;

			/* If the descriptor is ready, execute the command */
			if ((connfd > 0) && (FD_ISSET(connfd, &pool.ready_set))) {
				pool.nready--;
				if ((buflen = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
					printf("Server received %d bytes\n", buflen);
					execute_command(connfd, buflen, buf, &pool);
				}
				/* EOF detected, remove descriptor from pool */
				else {
					Close(connfd);
					FD_CLR(connfd, &pool.read_set);
					delete_fd(connfd, &pool);
					upload_file();
				}
			}
			/* If listening descriptor ready, add new client to pool */
			else if ((FD_ISSET(connfd, &pool.ready_set))) {
				clientlen = sizeof(struct sockaddr_storage);
				connfd = Accept(pool.listenfd, (SA *)&clientaddr, &clientlen);
				Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
				printf("Connected to (%s, %s)\n", client_hostname, client_port);
				FD_SET(connfd, &pool.read_set); //TODO: 이게맞나....
				add_fd(connfd, &pool);
			}
		}
	}
	exit(0);
}
/* $end stockserver main */
