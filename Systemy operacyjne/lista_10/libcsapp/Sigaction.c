#include <csapp.h>

void Sigaction(int signum, const struct sigaction *act,
               struct sigaction *oldact) {
  if (sigaction(signum, act, oldact) < 0)
    unix_error("Sigaction error");
}
