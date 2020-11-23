#include "csapp.h"

void Sigsuspend(const sigset_t *mask) {
  if (sigsuspend(mask) == -1 && errno == EINTR)
    return;
  unix_error("Sigsuspend error");
}
