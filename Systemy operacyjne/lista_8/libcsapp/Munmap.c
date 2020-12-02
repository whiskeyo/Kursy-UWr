#include "csapp.h"

void Munmap(void *addr, size_t len) {
  if (munmap(addr, len) < 0)
    unix_error("Munmap error");
}
