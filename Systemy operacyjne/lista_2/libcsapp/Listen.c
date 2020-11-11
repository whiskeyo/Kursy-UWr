#include "csapp.h"

void Listen(int s, int backlog) {
  int rc = listen(s, backlog);
  if (rc < 0)
    unix_error("Listen error");
}
