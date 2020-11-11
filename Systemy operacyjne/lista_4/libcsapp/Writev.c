#include "csapp.h"

size_t Writev(int fd, const struct iovec *iov, int iovcnt) {
  ssize_t rc = writev(fd, iov, iovcnt);
  if (rc < 0)
    unix_error("Writev error");
  return rc;
}
