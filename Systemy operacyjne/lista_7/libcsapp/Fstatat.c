#include "csapp.h"

void Fstatat(int dirfd, const char *pathname, struct stat *statbuf, int flags) {
  if (fstatat(dirfd, pathname, statbuf, flags) < 0)
    unix_error("Fstatat error");
}
