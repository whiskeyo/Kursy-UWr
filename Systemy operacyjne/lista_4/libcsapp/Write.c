#include "csapp.h"

size_t Write(int fd, const void *buf, size_t count) {
  ssize_t rc = write(fd, buf, count);
  if (rc < 0)
    unix_error("Write error");
  return rc;
}
