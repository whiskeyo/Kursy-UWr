#include "csapp.h"

void *Malloc(size_t size) {
  void *p = malloc(size);
  if (!p)
    unix_error("Malloc error");
  return p;
}

void *Realloc(void *ptr, size_t size) {
  void *p = realloc(ptr, size);
  if (!p)
    unix_error("Realloc error");
  return p;
}

void *Calloc(size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (!p)
    unix_error("Calloc error");
  return p;
}
