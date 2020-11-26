#include "csapp.h"

pid_t Tcgetpgrp(int fd) {
  int rc = tcgetpgrp(fd);
  if (rc < 0)
    unix_error("Tcgetpgrp error");
  return rc;
}
