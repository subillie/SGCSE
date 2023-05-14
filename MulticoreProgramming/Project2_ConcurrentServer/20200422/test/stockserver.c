/* 
* stockserver.c - A concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

static void sigint_handler(int sig) {

	upload_file();
	free_tree(root);
	exit(0);
}

/* $begin stockserver main */
int main(int argc, char **argv) {

	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr; /* Enough space for any address */ //line:netp:stockserverc:sockaddrstorage
	char client_hostname[MAXLINE], client_port[MAXLINE];
	static pool_t pool;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	Signal(SIGINT, sigint_handler);
	byte_cnt = 0;
	listenfd = Open_listenfd(argv[1]);
	init_pool(listenfd, &pool);
	init_items();

	while (1) {
		/* Wait for listening/connected descriptor(s) to become ready */
		pool.ready_set = pool.read_set;
		pool.nready = Select(pool.maxi + 1, &pool.ready_set, NULL, NULL, NULL);
		printf("nready: %d\n", pool.nready);
		/* If listening descriptor ready, add new client to pool */
		if ((FD_ISSET(listenfd, &pool.ready_set))) {
			printf("got client\n");
			clientlen = sizeof(struct sockaddr_storage);
			connfd = Accept(pool.maxi, (SA *)&clientaddr, &clientlen);
			Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
			printf("Connected to (%s, %s)\n", client_hostname, client_port);
			// FD_SET(connfd, &pool.read_set); //TODO: 이게맞나....
			add_client(connfd, &pool);
		}
		/* If the descriptor is ready, execute the command */
		check_clients(&pool);
	}
	exit(0);
}
/* $end stockserver main */
