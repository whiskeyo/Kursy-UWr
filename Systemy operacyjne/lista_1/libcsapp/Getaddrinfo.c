#include "csapp.h"

void Getaddrinfo(const char *node, const char *service,
                 const struct addrinfo *hints, struct addrinfo **res) {
  int rc = getaddrinfo(node, service, hints, res);

  if (rc != 0)
    gai_error(rc, "Getaddrinfo error");
}
