#include "csapp.h"

void Setpgid(pid_t pid, pid_t pgid) {
  if (setpgid(pid, pgid) < 0)
    unix_error("Setpgid error");
}
