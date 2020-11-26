#include "csapp.h"

void Madvise(void *addr, size_t length, int advice) {
  if (madvise(addr, length, advice) < 0)
    unix_error("Madvise error");
}
