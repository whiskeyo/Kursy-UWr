#include "csapp.h"

int Open(const char *pathname, int flags, mode_t mode) {
  int rc = open(pathname, flags, mode);
  if (rc < 0)
    unix_error("Open error");
  return rc;
}
