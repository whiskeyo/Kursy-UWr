#include "csapp.h"

void Close(int fd) {
  int rc = close(fd);
  if (rc < 0)
    unix_error("Close error");
}
