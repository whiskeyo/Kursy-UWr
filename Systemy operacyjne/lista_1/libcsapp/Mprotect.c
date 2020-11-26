#include "csapp.h"

void Mprotect(void *addr, size_t length, int prot) {
  if (mprotect(addr, length, prot) < 0)
    unix_error("Mprotect error");
}
