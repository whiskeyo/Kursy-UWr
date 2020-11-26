#include "csapp.h"

void Kill(pid_t pid, int sig) {
  if (kill(pid, sig) < 0)
    unix_error("Kill error");
}
