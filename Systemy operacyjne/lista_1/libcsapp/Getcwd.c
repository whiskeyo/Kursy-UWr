#include "csapp.h"

char *Getcwd(char *buf, size_t buflen) {
  char *res = getcwd(buf, buflen);
  if (res == NULL)
    unix_error("Getcwd failed");
  return res;
}
