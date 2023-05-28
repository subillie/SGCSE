/* 
* server_utils.c - utility functions for the concurrent stock server 
*/ 
#include "csapp.h"
#include "stockserver.h"

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

/* Create an empty, bounded, shared FIFO buffer with n slots */
void sbuf_init(sbuf_t *sp, int n) {

	sp->buf = Calloc(n, sizeof(int));
	sp->n = n;					/* Buffer holds max of n clients */
	sp->front = sp->rear = 0;	/* Empty buffer iff front == rear */
	Sem_init(&sp->mutex, 0, 1); /* Binary semaphore for locking */
	Sem_init(&sp->slots, 0, n); /* Initially, buf has n empty slots */
	Sem_init(&sp->clients, 0, 0); /* Initially, buf has zero data clients */
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
	V(&sp->clients);
}

int sbuf_remove(sbuf_t *sp) {

	P(&sp->clients);
	P(&sp->mutex);
	int connfd = sp->buf[(++sp->front) % (sp->n)];
	V(&sp->mutex);
	V(&sp->slots);
	return connfd;
}

static void show(item_t *ptr, int connfd) {

	FILE *fp = fopen("stock.txt", "r");
	char *tmp = (char *)Calloc(MAXLINE, sizeof(char));
	char *line = (char *)Calloc(MAXLINE, sizeof(char));
	int len = 0;

	/* Reader's lock */
	P(&ptr->mutex);
	ptr->readcnt++;
	if (ptr->readcnt == 1)
		P(&ptr->writer);
	V(&ptr->mutex);

	/* $begin critical section */
	while (fgets(tmp, MAXLINE, fp) != NULL) {
		strcat(line + len, tmp);
		len += strlen(tmp);
		memset(tmp, 0, MAXLINE);
	}
	/* $end critical section */

	/* Reader's unlock */
	P(&ptr->mutex);
	ptr->readcnt--;
	if (ptr->readcnt == 0)
		V(&ptr->writer);
	V(&ptr->mutex);

	fclose(fp);
	Rio_writen(connfd, line, MAXLINE);
	free(tmp);
	free(line);
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
	else { /* If found, but there is not enough stock, return */
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
		show(root, connfd);
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
