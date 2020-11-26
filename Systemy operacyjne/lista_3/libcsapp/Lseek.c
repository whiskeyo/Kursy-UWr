#include "csapp.h"

off_t Lseek(int fildes, off_t offset, int whence) {
  off_t rc = lseek(fildes, offset, whence);
  if (rc < 0)
    unix_error("Lseek error");
  return rc;
}
