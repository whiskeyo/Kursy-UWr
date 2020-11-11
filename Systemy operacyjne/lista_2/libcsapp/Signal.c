#include "csapp.h"

void (*Signal(int signum, void (*handler)(int)))(int) {
  void (*res)(int) = signal(signum, handler);
  if (res == SIG_ERR)
    unix_error("Signal error");
  return res;
}
