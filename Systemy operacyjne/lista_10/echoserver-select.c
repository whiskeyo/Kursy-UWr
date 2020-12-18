#include "csapp.h"
#include "rio.h"

#define LISTENQ 10

/* Represents a pool of connected descriptors */
typedef struct pool {
  int maxfd;                   /* Largest descriptor in read_set */
  fd_set read_set;             /* Set of all active descriptors */
  fd_set ready_set;            /* Subset of descriptors ready for reading  */
  int nready;                  /* Number of ready descriptors from select */
  int maxi;                    /* Highwater index into client array */
  int clientfd[FD_SETSIZE];    /* Set of active descriptors */
  rio_t clientrio[FD_SETSIZE]; /* Set of active read buffers */
} pool_t;

static int byte_cnt = 0; /* Counts total bytes received by server */

static void init_pool(int listenfd, pool_t *p) {
  /* Initially, there are no connected descriptors */
  p->maxi = -1;
  for (int i = 0; i < FD_SETSIZE; i++)
    p->clientfd[i] = -1;

  /* Initially, listenfd is only member of select read set */
  p->maxfd = listenfd;
  FD_ZERO(&p->read_set);
  FD_SET(listenfd, &p->read_set);
}

static void add_client(int connfd, pool_t *p) {
  p->nready--;

  for (int i = 0; i < FD_SETSIZE; i++) {
    /* Find an available slot */
    if (p->clientfd[i] >= 0)
      continue;

    /* Add connected descriptor to the pool */
    p->clientfd[i] = connfd;
    rio_readinitb(&p->clientrio[i], connfd);

    /* Add the descriptor to descriptor set */
    FD_SET(connfd, &p->read_set);

    /* Update max descriptor and pool highwater mark */
    if (connfd > p->maxfd)
      p->maxfd = connfd;
    if (i > p->maxi)
      p->maxi = i;
    return;
  }

  app_error("add_client error: Too many clients");
}

static void check_clients(pool_t *p) {
  char buf[MAXLINE];
  int n;

  for (int i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
    int connfd = p->clientfd[i];
    rio_t *rio = &p->clientrio[i];

    /* If the descriptor is ready, echo a text line from it */
    if (connfd > 0 && FD_ISSET(connfd, &p->ready_set)) {
      p->nready--;
      if ((n = Rio_readlineb(rio, buf, MAXLINE)) != 0) {
        byte_cnt += n;
        printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt,
               connfd);
        Rio_writen(connfd, buf, n);
      } else {
        /* EOF detected, remove descriptor from pool */
        Close(connfd);
        FD_CLR(connfd, &p->read_set);
        p->clientfd[i] = -1;
      }
    }
  }
}

static pool_t pool;

int main(int argc, char **argv) {
  int connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  if (argc != 2)
    app_error("usage: %s <port>\n", argv[0]);

  int listenfd = Open_listenfd(argv[1], LISTENQ);
  init_pool(listenfd, &pool);

  while (1) {
    /* Wait for listening/connected descriptor(s) to become ready */
    pool.ready_set = pool.read_set;
    pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

    /* If listening descriptor ready, add new client to pool */
    if (FD_ISSET(listenfd, &pool.ready_set)) {
      clientlen = sizeof(struct sockaddr_storage);
      connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
      add_client(connfd, &pool);
    }

    /* Echo a text line from each ready connected descriptor */
    check_clients(&pool);
  }
}
