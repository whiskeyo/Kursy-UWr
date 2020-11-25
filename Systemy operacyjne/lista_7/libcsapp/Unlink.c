#include "csapp.h"

void Unlink(const char *pathname) {
  if (unlink(pathname) < 0)
    unix_error("Unlink error");
}
