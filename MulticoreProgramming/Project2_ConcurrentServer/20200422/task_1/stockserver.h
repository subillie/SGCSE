/*
* stockserver.h - stock server header file
*/
/* $begin stockserver.h */
#ifndef __STOCKSERVER_H__
#define __STOCKSERVER_H__

/* 주식 종목 리스트 */
typedef struct item_s {
	int id;
	int quantity;
	int price;
	struct item_s *left;
	struct item_s *right;
}	item_t;
item_t *root;

typedef struct pool_s {
	int maxfd;					 /* Largest descriptor in read_set */
	fd_set read_set;			 /* Set of all active descriptors */
	fd_set ready_set;			 /* Subset of descriptors ready for reading */
	int nready;					 /* Number of ready descriptors from select */
	int maxi;					 /* High water index into client array */
	int clientfd[FD_SETSIZE];	 /* Set of active descriptors and read buffers */
	rio_t clientrio[FD_SETSIZE]; /* Set of active read buffers */
}	pool_t;

int nstock;	/* Number of stock */

/* function prototypes - server_pool.c */
void init_pool(int listenfd, pool_t *pool);
void add_client(int connfd, pool_t *pool);
void check_clients(pool_t *pool);
void execute_command(int i, int connfd, int buflen, char *buf, pool_t *pool);

/* function prototypes - server_item.c */
void update_textfile();
void add_item(item_t **ptr, int id, int quantity, int price);
void init_items();
void free_tree(item_t *ptr);
item_t *find(int id);
void print_fp(item_t *ptr, FILE *fp);

#endif /* __STOCKSERVER_H__ */
/* $end stockserver.h */
