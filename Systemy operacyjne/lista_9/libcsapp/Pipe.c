#include "csapp.h"

void Pipe(int fds[2]) {
  if (pipe(fds) < 0)
    unix_error("Pipe error");
}
