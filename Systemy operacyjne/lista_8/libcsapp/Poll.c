#include "csapp.h"

int Poll(struct pollfd *fds, nfds_t nfds, int timeout) {
  int rc = poll(fds, nfds, timeout);
  if (rc == -1 && errno == EINTR)
    rc = 0;
  if (rc < 0)
    unix_error("Poll error");
  return rc;
}
