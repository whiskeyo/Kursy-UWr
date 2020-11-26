#include "csapp.h"

#ifdef LINUX
#include <asm/unistd.h>

int Getdents(int fd, struct linux_dirent *dirp, unsigned count) {
  int rc = syscall(__NR_getdents, fd, dirp, count);
  if (rc < 0)
    unix_error("Getdents error");
  return rc;
}
#endif
