#include "csapp.h"
#include "rio.h"

#define LISTENQ 1

static size_t nread = 0;

static void sigint_handler(int sig) {
  /* TODO: Change control flow so that it does not return to main loop. */
  safe_printf("\nTotal bytes read: %ld.\n", nread);
  _Exit(0);
}

static void echo(int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  rio_readinitb(&rio, connfd);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE))) {
    Rio_writen(connfd, buf, n);
    nread += n;
  }
}

int main(int argc, char **argv) {
  if (argc != 2)
    app_error("usage: %s <port>\n", argv[0]);

  Signal(SIGINT, sigint_handler);

  int listenfd = Open_listenfd(argv[1], LISTENQ);

  /* TODO: Print bytes received after SIGINT has been received. */
  // This is done in sigint_handler by printing the value of
  // global variable nread, which gets changed in echo function.

  while (1) {
    socklen_t clientlen = sizeof(struct sockaddr_storage);
    struct sockaddr_storage clientaddr; /* Enough space for any address */
    char client_hostname[MAXLINE], client_port[MAXLINE];
    int connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("Connected to %s:%s\n", client_hostname, client_port);
    echo(connfd);
    Close(connfd);
  }
}
