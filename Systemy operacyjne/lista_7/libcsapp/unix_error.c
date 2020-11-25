#include <stdarg.h>
#include "csapp.h"

void unix_error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, ": %s\n", strerror(errno));
  va_end(ap);
  exit(EXIT_FAILURE);
}
