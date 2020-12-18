#include "csapp.h"

int Socket(int domain, int type, int protocol) {
  int rc = socket(domain, type, protocol);
  if (rc < 0)
    unix_error("Socket error");
  return rc;
}
