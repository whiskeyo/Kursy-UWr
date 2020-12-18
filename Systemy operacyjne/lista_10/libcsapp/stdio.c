#include "csapp.h"

char *Fgets(char *ptr, int n, FILE *stream) {
  char *rptr;

  if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
    app_error("Fgets error");

  return rptr;
}

void Fputs(const char *ptr, FILE *stream) {
  if (fputs(ptr, stream) == EOF)
    unix_error("Fputs error");
}
