#include "csapp.h"

void *Mmap(void *addr, size_t length, int prot, int flags, int fd,
           off_t offset) {
  void *ptr = mmap(addr, length, prot, flags, fd, offset);
  if (ptr == MAP_FAILED)
    unix_error("Mmap error");
  return ptr;
}
