#include "csapp.h"

void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  if (sigprocmask(how, set, oldset) < 0)
    unix_error("Sigprocmask error");
}
