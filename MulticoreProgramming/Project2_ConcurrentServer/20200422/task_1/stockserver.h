/*
* stockserver.h - stock server header file
*/
/* $begin stockserver.h */
#ifndef __STOCKSERVER_H__
#define __STOCKSERVER_H__

typedef struct pool_s {
	int maxfd;					 /* Largest descriptor in read_set */
	fd_set read_set;			 /* Set of all active descriptors */
	fd_set ready_set;			 /* Subset of descriptors ready for reading */
	int nready;					 /* Number of ready descriptors from select */
	int maxi;					 /* Highwater index into client array */
	int clientfd[FD_SETSIZE];	 /* Set of active descriptors */
	rio_t clientrio[FD_SETSIZE]; /* Set of active read buffers; buffered I/O for each client */
}	pool_t;

/* 주식 종목 리스트 */
typedef struct stock_item_s *stock_item_list;
typedef struct stock_item_s {
	int id;
	int left_stock;
	int price;
	int readcnt;
	sem_t mutex;
	sem_t writer;
	stock_item_list next;
	stock_item_list left;
	stock_item_list right;
}	stock_item_t;

stock_item_list *root = NULL;
stock_item_list *head = NULL;
stock_item_list *tail = NULL;
int stock_count = 0; /* 주식 종목 개수 */

/* 주식 거래 요청 리스트*/
// typedef struct request_s *request_list;
// typedef struct request_s {
// 	int fd;
// 	request_list next;
// }	request_t;

// request_list *request_head = NULL;
// request_list *request_tail = NULL;

#endif /* __STOCKSERVER_H__ */
/* $end stockserver.h */
