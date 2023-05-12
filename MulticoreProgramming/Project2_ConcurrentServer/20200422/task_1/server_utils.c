/* 
* server_utils.c - Utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

void init_pool(int listenfd, pool_t *pool) {

	/* Initially, there are no connected descriptors */
	int i;
	pool->maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		pool->clientfd[i] = -1;

	/* Initially, listenfd is only member of select read set */
	pool->maxfd = listenfd;
	FD_ZERO(&pool->read_set);
	FD_SET(listenfd, &pool->read_set);
}

void add_client(int connfd, pool_t *pool) {

	int i;
	pool->nready--;
	for (i = 0; i < FD_SETSIZE; i++) { /* Find an available slot */
		if (pool->clientfd[i] < 0) {
			/* Add connected descriptor to the pool */
			pool->clientfd[i] = connfd;
			Rio_readinitb(&pool->clientrio[i], connfd);

			/* Add the descriptor to descriptor set */
			FD_SET(connfd, &pool->read_set);

			/* Update max descriptor and pool highwater mark */
			if (pool->maxfd < connfd)
				pool->maxfd = connfd;
			if (pool->maxi < i)
				pool->maxi = i;
			break;
		}
	}
	if (i == FD_SETSIZE) /* Couldn't find an empty slot */
		app_error("add_client error: Too many clients");
}

void show(int connfd) {

	char list[MAXLINE];

	list[0] = '\0';
	traverse(root, list);
	strcat(list, "\n");
	Rio_writen(connfd, list, strlen(list));
}

void buy(int connfd, int id, int count) {

	/* Find the node with the given id */
	stock_item_t *ptr = find(id);

	/* If the node is not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}

	/* If the node is found, update the stock */
	if (ptr->left_stock >= count) {
		ptr->left_stock -= count;
		Rio_writen(connfd, "[buy] success\n", 14);
	}
	else /* If there is not enough stock, return */
		Rio_writen(connfd, "Not enough left stock\n", 22);
}

void sell(int connfd, int id, int count) {

	/* Find the node with the given id */
	stock_item_t *ptr = find(id);

	/* If not found, return */
	if (ptr == NULL) {
		Rio_writen(connfd, "Invalid ID\n", 11);
		return;
	}
	/* If found, update the stock */
	ptr->left_stock += count;
	Rio_writen(connfd, "[sell] success\n", 15);
}

void execute_command(int i, int connfd, int buflen, char *buf, pool_t pool) {

	int id, count;
	char command[MAXLINE];

	if (!strcmp(buf, "exit\n")) {
		Close(connfd);
		FD_CLR(connfd, &pool.read_set);
		pool.clientfd[i] = -1;
		Rio_writen(connfd, buf, buflen);
		upload_file();
	}
	else if (!strcmp(buf, "\n"))
		Rio_writen(connfd, buf, buflen);
	else {
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
}
