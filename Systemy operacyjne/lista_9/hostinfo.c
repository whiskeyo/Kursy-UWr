#include "csapp.h"

int main(int argc, char **argv) {
  struct addrinfo *p, *listp, hints;
  char buf[MAXLINE], srv[MAXLINE];
  int rc, flags;

  // strings for arguments given by argv[]
  const char *domain, *service;

  if (argc == 2) { // default ./hostinfo <domain> run
    domain  = argv[1];
    service = NULL;
  } else if (argc == 3) { // handling optional argument
    domain  = argv[1];
    service = argv[2];
  } else {
    app_error("usage: %s <domain name> [service (optional)]\n", argv[0]);
  }

  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family   = AF_UNSPEC;    // changed from AF_INET to AF_UNSPEC to handle IPv4 and IPv6 
  hints.ai_socktype = 0;  // changed from SOCK_STREAM to handle tftp
  
  /* Connections only */
  // Changed NULL to service to get info about it
  if ((rc = getaddrinfo(domain, service, &hints, &listp)) != 0)
    gai_error(rc, "getaddrinfo");

  /* Walk the list and display each IP address */
  flags = NI_NUMERICHOST | NI_NUMERICSERV; /* Display address string instead of domain name */
  for (p = listp; p; p = p->ai_next) {
    Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, srv, MAXLINE, flags);
    
    if (service && p->ai_family == AF_INET6) // handle IPv6
      printf("[%s]:%s\n", buf, srv);
    else if (service)
      printf("%s:%s\n", buf, srv);
    else
      printf("%s\n", buf);
  }

  /* Clean up */
  freeaddrinfo(listp);

  return EXIT_SUCCESS;
}
