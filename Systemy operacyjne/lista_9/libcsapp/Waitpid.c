#include "csapp.h"

pid_t Waitpid(pid_t pid, int *iptr, int options) {
  pid_t retpid;
  if ((retpid = waitpid(pid, iptr, options)) < 0)
    unix_error("Waitpid error");
  return retpid;
}
