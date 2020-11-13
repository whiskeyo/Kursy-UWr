#include "csapp.h"

int Dup(int fd) {
  int rc = dup(fd);
  if (rc < 0)
    unix_error("Dup error");
  return rc;
}
