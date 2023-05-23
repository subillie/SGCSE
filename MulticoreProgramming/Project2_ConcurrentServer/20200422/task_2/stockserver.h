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

typedef struct {
	int *buf;	 /* Buffer array */
	int n;		 /* Maximum number of slots */
	int front;	 /* buf[(front+1)%n] is first item */
	int rear;	 /* buf[rear%n] is last item */
	sem_t mutex; /* Protects accesses to buf */
	sem_t slots; /* Counts available slots */
	sem_t items; /* Counts available items */
}	sbuf_t;

int nstock;	/* Number of stock */

/* function prototypes - server_sp.c */
void *thread(void *vargp, int connfd);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);

#endif /* __STOCKSERVER_H__ */
/* $end stockserver.h */
