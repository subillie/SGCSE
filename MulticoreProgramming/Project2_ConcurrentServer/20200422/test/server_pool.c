/* 
* server_utils.c - Utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void init_pool(int listenfd, pool_t *pool) {

	/* Initially, there are no connected descriptors */
	pool->maxi = -1;
	for (int i = 0; i < FD_SETSIZE; i++)
		pool->clientfd[i] = -1;

	/* Initially, listenfd is only member of select read set */
	pool->maxfd = listenfd;
	pool->nready = 0;
	FD_ZERO(&pool->read_set);
	FD_SET(listenfd, &pool->read_set);
}

void add_client(int connfd, pool_t *pool) {

	int i;

	pool->nready--;
	/* Find an available slot in the clientfd array */
	for (i = 0; i < FD_SETSIZE; i++) {
		if (pool->clientfd[i] < 0) {
			/* Add connected descriptor to the pool */
			pool->clientfd[i] = connfd;
			Rio_readinitb(&pool->clientrio[i], connfd);

			/* Add the descriptor to the descriptor set */
			FD_SET(connfd, &pool->read_set);

			/* Update max descriptor and pool high water mark */
			if (pool->maxfd < connfd)
				pool->maxfd = connfd;
			if (pool->maxi < i)
				pool->maxi = i;
			break;
		}
	}
	/* Couldn't find an empty slot */
	if (i == FD_SETSIZE)
		app_error("add_client error: Too many clients");
}

void check_clients(pool_t *pool) {

	int connfd, n;
	char buf[MAXLINE];
	rio_t rio;

	/* Check for data from connected clients */
	for (int i = 0; (i <= pool->maxi) && (pool->nready > 0); i++) {
		connfd = pool->clientfd[i];
		rio = pool->clientrio[i];

		/* If the descriptor is ready, echo a text line from it */
		if ((connfd > 0) && (FD_ISSET(connfd, &pool->ready_set))) {
			pool->nready--;
			if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
				// byte_cnt += n;
				// printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, connfd);
				printf("Server received %d bytes\n", n);
				execute_command(i, connfd, n, buf, pool);
			}
			/* EOF detected, remove descriptor from pool */
			else {
				Close(connfd);
				FD_CLR(connfd, &pool->read_set);
				pool->clientfd[i] = -1;
			}
		}
	}
}

static void print_rio(item_t *ptr, char *list) {

	if (ptr == NULL) {
		return;
	}
	print_rio(ptr->left, list);
	sprintf(list + strlen(list), "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
	print_rio(ptr->right, list);
}

static void show(int connfd) {

	char list[MAXLINE];
	list[0] = '\0';
	print_rio(root, list);

	size_t list_len = strlen(list);
	// for (size_t i = 0; i < list_len; i++) {
	// 	write(connfd, &list[i], 1);
	// 	// Rio_writen(connfd, &list[i], 1);
	// }
	Rio_writen(connfd, list, list_len);
	Rio_writen(connfd, "\n", 1);
}


static void buy(int connfd, int id, int count) {

	/* Find the given id in tree */
	item_t *ptr = find(id);

	/* If the node is not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}

	/* If the node is found, update the stock */
	if (ptr->quantity >= count) {
		ptr->quantity -= count;
		Rio_writen(connfd, "[buy] success\n", 14);
	}
	else /* If there is not enough stock, return */
		Rio_writen(connfd, "Not enough left stock\n", 22);
}

static void sell(int connfd, int id, int count) {

	/* Find the node with the given id */
	item_t *ptr = find(id);

	/* If not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}
	/* If found, update the stock */
	ptr->quantity += count;
	Rio_writen(connfd, "[sell] success\n", 15);
}

void execute_command(int i, int connfd, int buflen, char *buf, pool_t *pool) {

	int id, count;
	char command[MAXLINE];

	if (!strcmp(buf, "exit\n")) {
		upload_file();
		Close(connfd); /* Close the connection */
		FD_CLR(connfd, &pool->read_set);
		pool->clientfd[i] = -1;
		return;        /* Return after closing the connection */
	}
	sscanf(buf, "%s %d %d", command, &id, &count);
	if (!strcmp(command, "show"))
		show(connfd);
	else if (!strcmp(command, "buy"))
		buy(connfd, id, count);
	else if (!strcmp(command, "sell"))
		sell(connfd, id, count);
	else
		Rio_writen(connfd, "Invalid command\n", 16);
}
