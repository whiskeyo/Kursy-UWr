#include "csapp.h"

void Setsockopt(int s, int level, int optname, const void *optval, int optlen) {
  int rc = setsockopt(s, level, optname, optval, optlen);
  if (rc < 0)
    unix_error("Setsockopt error");
}
