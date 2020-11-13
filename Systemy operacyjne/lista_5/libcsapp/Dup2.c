#include "csapp.h"

int Dup2(int oldfd, int newfd) {
  int rc = dup2(oldfd, newfd);
  if (rc < 0)
    unix_error("Dup2 error");
  return rc;
}
