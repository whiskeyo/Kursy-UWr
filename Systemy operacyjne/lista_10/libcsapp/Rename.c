#include "csapp.h"

void Rename(const char *oldpath, const char *newpath) {
  if (rename(oldpath, newpath) < 0)
    unix_error("Rename error");
}
