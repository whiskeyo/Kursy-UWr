#include "csapp.h"

void Socketpair(int domain, int type, int protocol, int sv[2]) {
  if (socketpair(domain, type, protocol, sv) < 0)
    unix_error("Socketpair error");
}
