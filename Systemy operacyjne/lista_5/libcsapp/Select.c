#include "csapp.h"

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout) {
  int rc = select(n, readfds, writefds, exceptfds, timeout);
  if (rc < 0)
    unix_error("Select error");
  return rc;
}
