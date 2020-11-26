#include "csapp.h"

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
  int rc = accept(s, addr, addrlen);
  if (rc < 0)
    unix_error("Accept error");
  return rc;
}
