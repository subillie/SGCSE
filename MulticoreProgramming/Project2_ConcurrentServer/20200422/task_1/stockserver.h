/*
* stockserver.h - stock server header file
*/
/* $begin stockserver.h */
#ifndef __STOCKSERVER_H__
#define __STOCKSERVER_H__

/* 주식 종목 리스트 */
typedef struct stock_s {
	int id;
	int quantity;
	int price;
	struct stock_s *next;
	struct stock_s *left;
	struct stock_s *right;
}	stock_t;
stock_t *root;

/* 클라이언트 리스트 */
typedef struct client_s {
	int fd;		/* Active descriptor */
	rio_t rio;	/* Buffered I/O for each client */
	struct client_s *next;
}	client_t;
client_t *client_head;
client_t *client_tail;

typedef struct pool_s {
	int maxfd;			/* Largest descriptor in read_set */
	fd_set read_set;	/* Set of all active descriptors */
	fd_set ready_set;	/* Subset of descriptors ready for reading */
	int nready;			/* Number of ready descriptors from select */
	int maxi;			/* High water index into client array */
	int nstock;			/* Number of stock */
	client_t *client;	/* Set of active descriptors and read buffers */
}	pool_t;

/* function prototypes - server_utils.c */
void init_pool(int listenfd, pool_t *pool);
void add_fd(int connfd, pool_t *pool);
void delete_fd(int connfd, pool_t *pool);
void execute_command(int connfd, int buflen, char *buf, pool_t *pool);

/* function prototypes - server_tree.c */
void free_tree(stock_t *ptr);
void traverse(stock_t *ptr, char *list);
stock_t *find(int id);
void print_inorder(stock_t *ptr, FILE *fp);
void upload_file();

#endif /* __STOCKSERVER_H__ */
/* $end stockserver.h */
