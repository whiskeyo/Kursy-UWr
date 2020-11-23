#include "csapp.h"

void Tcsetpgrp(int fd, pid_t pgrp) {
  int rc = tcsetpgrp(fd, pgrp);
  if (rc < 0)
    unix_error("Tcsetpgrp error");
}
