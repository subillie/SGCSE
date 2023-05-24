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
	int readcnt;
	sem_t mutex;
	sem_t writer;
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

int nstock;			/* Number of stock */
sem_t file_mutex;	/* Protects accesses to stock.txt */
sbuf_t sbuf;		/* Shared buffer of connected descriptors */

/* function prototypes - server_sp.c */
void *thread(void *vargp);
void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int connfd);
int sbuf_remove(sbuf_t *sp);
void execute_command(int connfd, char *buf);

/* function prototypes - server_item.c */
void update_textfile();
void add_item(item_t **ptr, int id, int quantity, int price);
void init_items();
void free_tree(item_t *ptr);
item_t *find(int id);
void print_fp(item_t *ptr, FILE *fp);

#endif /* __STOCKSERVER_H__ */
/* $end stockserver.h */
