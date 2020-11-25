#include "csapp.h"

size_t Read(int fd, void *buf, size_t count) {
  ssize_t rc = read(fd, buf, count);
  if (rc < 0)
    unix_error("Read error");
  return rc;
}
