#include "csapp.h"

void Ftruncate(int fd, off_t length) {
  if (ftruncate(fd, length) < 0)
    unix_error("Ftruncate error");
}
