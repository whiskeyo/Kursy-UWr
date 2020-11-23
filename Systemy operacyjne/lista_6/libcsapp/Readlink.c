#include "csapp.h"

size_t Readlink(const char *pathname, char *buf, size_t bufsiz) {
  int rc = readlink(pathname, buf, bufsiz);
  if (rc < 0)
    unix_error("Readlink error");
  return rc;
}
