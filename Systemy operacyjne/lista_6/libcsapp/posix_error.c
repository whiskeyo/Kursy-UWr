#include <stdarg.h>
#include "csapp.h"

void posix_error(int code, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, ": %s\n", strerror(code));
  va_end(ap);
  exit(EXIT_FAILURE);
}
