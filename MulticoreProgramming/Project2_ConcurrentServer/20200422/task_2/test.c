#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

/* Default file permissions are DEF_MODE & ~DEF_UMASK */
/* $begin createmasks */
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH
/* $end createmasks */

/* Simplifies calls to bind(), connect(), and accept() */
/* $begin sockaddrdef */
typedef struct sockaddr SA;
/* $end sockaddrdef */

/* Persistent state for the robust I/O (Rio) package */
/* $begin rio_t */
#define RIO_BUFSIZE 8192
typedef struct {
	int rio_fd;                /* Descriptor for this internal buf */
	int rio_cnt;               /* Unread bytes in internal buf */
	char *rio_bufptr;          /* Next unread byte in internal buf */
	char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;
/* $end rio_t */

/* External variables */
extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/****************************************
 * The Rio package - Robust I/O functions
 ****************************************/

/*
* rio_readn - Robustly read n bytes (unbuffered)
*/
/* $begin rio_readn */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;

	while (nleft > 0) {
	if ((nread = read(fd, bufp, nleft)) < 0) {
		if (errno == EINTR) /* Interrupted by sig handler return */
		nread = 0;      /* and call read() again */
		else
		return -1;      /* errno set by read() */ 
	} 
	else if (nread == 0)
		break;              /* EOF */
	nleft -= nread;
	bufp += nread;
	}
	return (n - nleft);         /* return >= 0 */
}
/* $end rio_readn */

/*
* rio_writen - Robustly write n bytes (unbuffered)
*/
/* $begin rio_writen */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = usrbuf;

	while (nleft > 0) {
	if ((nwritten = write(fd, bufp, nleft)) <= 0) {
		if (errno == EINTR)  /* Interrupted by sig handler return */
		nwritten = 0;    /* and call write() again */
		else
		return -1;       /* errno set by write() */
	}
	nleft -= nwritten;
	bufp += nwritten;
	}
	return n;
}
/* $end rio_writen */


/* 
* rio_read - This is a wrapper for the Unix read() function that
*    transfers min(n, rio_cnt) bytes from an internal buffer to a user
*    buffer, where n is the number of bytes requested by the user and
*    rio_cnt is the number of unread bytes in the internal buffer. On
*    entry, rio_read() refills the internal buffer via a call to
*    read() if the internal buffer is empty.
*/
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
	rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
			sizeof(rp->rio_buf));
	if (rp->rio_cnt < 0) {
		if (errno != EINTR) /* Interrupted by sig handler return */
		return -1;
	}
	else if (rp->rio_cnt == 0)  /* EOF */
		return 0;
	else 
		rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
	}

	/* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
	cnt = n;          
	if (rp->rio_cnt < n)   
	cnt = rp->rio_cnt;
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;
}
/* $end rio_read */

/*
* rio_readinitb - Associate a descriptor with a read buffer and reset buffer
*/
/* $begin rio_readinitb */
void rio_readinitb(rio_t *rp, int fd) 
{
	rp->rio_fd = fd;  
	rp->rio_cnt = 0;  
	rp->rio_bufptr = rp->rio_buf;
}
/* $end rio_readinitb */

/*
* rio_readnb - Robustly read n bytes (buffered)
*/
/* $begin rio_readnb */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;
	
	while (nleft > 0) {
	if ((nread = rio_read(rp, bufp, nleft)) < 0) 
			return -1;          /* errno set by read() */ 
	else if (nread == 0)
		break;              /* EOF */
	nleft -= nread;
	bufp += nread;
	}
	return (n - nleft);         /* return >= 0 */
}
/* $end rio_readnb */

/* 
* rio_readlineb - Robustly read a text line (buffered)
*/
/* $begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
	int n, rc;
	char c, *bufp = usrbuf;

	for (n = 1; n < maxlen; n++) { 
		if ((rc = rio_read(rp, &c, 1)) == 1) {
		*bufp++ = c;
		if (c == '\n') {
				n++;
			break;
			}
	} else if (rc == 0) {
		if (n == 1)
		return 0; /* EOF, no data read */
		else
		break;    /* EOF, some data was read */
	} else
		return -1;	  /* Error */
	}
	*bufp = 0;
	return n-1;
}
/* $end rio_readlineb */

/**********************************
 * Wrappers for robust I/O routines
 **********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
	ssize_t n;

	if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
	return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) 
{
	if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
	rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
	ssize_t rc;

	if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
	return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
	ssize_t rc;

	if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
	return rc;
} 

/******************************** 
 * Client/server helper functions
 ********************************/
/*
* open_clientfd - Open connection to server at <hostname, port> and
*     return a socket descriptor ready for reading and writing. This
*     function is reentrant and protocol-independent.
* 
*     On error, returns -1 and sets errno.  
*/

int main() {

	rio_t rio;
	int n, connfd;
	char buf[MAXLINE];

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

	return 0;
}