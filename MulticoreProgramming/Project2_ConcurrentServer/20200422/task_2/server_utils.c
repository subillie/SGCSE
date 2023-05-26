/* 
* server_utils.c - utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

// void *thread(void *vargp) {

// 	Pthread_detach(pthread_self());

// 	rio_t rio;
// 	int n;
// 	int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
// 	char buf[MAXLINE];

// 	while (1) {
// 		Rio_readinitb(&rio, connfd);

// 		while (1) {
// 			memset(buf, 0, MAXLINE);
// 			if ((n = Rio_readnb(&rio, buf, MAXLINE)) == 0) {
// 				Close(connfd);
// 				break;
// 			}
// 			printf("server received %d bytes\n", n);
// 			execute_command(connfd, buf);
// 		}
// 	}

// 	return NULL;
// }

void *thread(void *vargp) {

	rio_t rio;
	int n, connfd;
	char buf[MAXLINE];

	Pthread_detach(pthread_self());
	connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
	Rio_readinitb(&rio, connfd);
	memset(buf, 0, MAXLINE);

	while (1) {
		if ((n = Rio_readlineb(&rio, buf, MAXLINE)) == 0) {
			Close(connfd);
			break;
		}
		printf("server received %d bytes\n", n);
		execute_command(connfd, buf);
		memset(buf, 0, MAXLINE);
		Rio_readinitb(&rio, connfd);
		memset(rio.rio_buf, 0, RIO_BUFSIZE);
	}

	return NULL;
}

// void *thread(void *vargp) {

// 	char buf[MAXLINE];
// 	rio_t rio;
// 	int n;

// 	Pthread_detach(pthread_self());
// 	int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
// 	while (1) {
// 		while (1)
// 		{
// 			memset(buf, 0, MAXLINE);
// 			Rio_readinitb(&rio, connfd);
// 			memset(rio.rio_buf, 0, MAXLINE);
// 			if ((n = Rio_readlineb(&rio, buf, MAXLINE)) == 0) {
// 				Close(connfd);
// 				break;
// 			}
// 			printf("server received %d bytes\n", n);
// 			execute_command(connfd, buf);
// 		}
// 	}
// 	return (NULL);
// }

// void *thread(void *vargp) {

// 	char buf[MAXLINE];
// 	rio_t rio;
// 	int n;

// 	Pthread_detach(pthread_self());
// 	while (1) {
// 		int connfd = sbuf_remove(&sbuf); // Remove connfd from buffer /
// 		memset(buf, 0, MAXLINE);
// 		Rio_readinitb(&rio, connfd);
// 			while (1) {
// 			if ((n = Rio_readlineb(&rio, buf, MAXLINE)) == 0)
// 			break;
// 			printf("server received %d bytes\n", n);
// 			execute_command(connfd, buf);
// 			memset(buf, 0, MAXLINE);
// 			}
// 		Close(connfd);
// 	}
// }

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

void sbuf_insert(sbuf_t *sp, int connfd) {

	P(&sp->slots);
	P(&sp->mutex);
	sp->buf[(++sp->rear) % (sp->n)] = connfd;
	V(&sp->mutex);
	V(&sp->items);
}

int sbuf_remove(sbuf_t *sp) {

	P(&sp->items);
	P(&sp->mutex);
	int connfd = sp->buf[(++sp->front) % (sp->n)];
	V(&sp->mutex);
	V(&sp->slots);
	return connfd;
}

static void print_rio(item_t *ptr, int connfd) {

	if (ptr == NULL)
		return;

	char list[MAXLINE];
	memset(list, 0, MAXLINE);
	print_rio(ptr->left, connfd);

	/* Reader's lock */
	P(&ptr->mutex);
	ptr->readcnt++;
	if (ptr->readcnt == 1)
		P(&ptr->writer);
	V(&ptr->mutex);

	/* $begin critical section */
	sprintf(list, "%d %d %d\n", ptr->id, ptr->quantity, ptr->price);
	Rio_writen(connfd, list, MAXLINE);
	/* $end critical section */

	/* Reader's unlock */
	P(&ptr->mutex);
	ptr->readcnt--;
	if (ptr->readcnt == 0)
		V(&ptr->writer);
	V(&ptr->mutex);

	print_rio(ptr->right, connfd);
}

static void show(int connfd) {

	char list[MAXLINE];
	memset(list, 0, MAXLINE);
	print_rio(root, connfd);
}

static void buy(int connfd, int id, int count) {

	/* Find the given id in tree */
	item_t *ptr = find(id);
	char *tmp = (char *)Calloc(MAXLINE, sizeof(char));

	/* If the node is not found, return */
	if (ptr == NULL) {
		strcpy(tmp, "Invalid ID\n");
		Rio_writen(connfd, tmp, MAXLINE);
		free(tmp);
		return;
	}
	/* Writer's lock */
	P(&ptr->writer);

	/* $begin critical section */
	/* If the node is found, update the stock */
	if (ptr->quantity >= count) {
		ptr->quantity -= count;
		strcpy(tmp, "[buy] success\n");
		Rio_writen(connfd, tmp, MAXLINE);
		free(tmp);
	}
	else { /* If there is not enough stock, return */
		strcpy(tmp, "Not enough left stock\n");
		Rio_writen(connfd, tmp, MAXLINE);
		free(tmp);
	}
	/* $end critical section */

	/* Writer's unlock */
	V(&ptr->writer);
}

static void sell(int connfd, int id, int count) {

	/* Find the node with the given id */
	item_t *ptr = find(id);
	char *tmp = (char *)Calloc(MAXLINE, sizeof(char));

	/* If not found, return */
	if (ptr == NULL) {
		strcpy(tmp, "Invalid ID\n");
		Rio_writen(connfd, tmp, MAXLINE);
		free(tmp);
		return;
	}

	/* Writer's lock */
	P(&ptr->writer);

	/* $begin critical section */
	/* If found, update the stock */
	ptr->quantity += count;
	strcpy(tmp, "[sell] success\n");
	Rio_writen(connfd, tmp, MAXLINE);
	free(tmp);
	/* $end critical section */

	/* Writer's unlock */
	V(&ptr->writer);
}

void execute_command(int connfd, char *buf) {

	int id, count;
	char command[MAXLINE];

	if (!strcmp(buf, "exit\n")) {
		update_textfile();
		Close(connfd);
		sbuf_deinit(&sbuf);
		free_tree(root);
		return;
	}
	sscanf(buf, "%s %d %d", command, &id, &count);
	if (!strcmp(command, "show"))
		show(connfd);
	else if (!strcmp(command, "buy")) {
		buy(connfd, id, count);
		update_textfile();
	}
	else if (!strcmp(command, "sell")) {
		sell(connfd, id, count);
		update_textfile();
	}
	else {
		char *tmp = (char *)Calloc(MAXLINE, sizeof(char));
		strcpy(tmp, "Invalid command\n");
		Rio_writen(connfd, tmp, MAXLINE);
		free(tmp);
	}
}
