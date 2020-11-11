#include "csapp.h"

size_t Readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz) {
  ssize_t rc = readlinkat(dirfd, pathname, buf, bufsiz);
  if (rc < 0)
    unix_error("Readlinkat error");
  return rc;
}
