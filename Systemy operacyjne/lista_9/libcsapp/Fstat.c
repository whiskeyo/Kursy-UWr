#include "csapp.h"

void Fstat(int fd, struct stat *statbuf) {
  if (fstat(fd, statbuf) < 0)
    unix_error("Fstat error");
}
