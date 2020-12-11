#include "csapp.h"

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                 size_t hostlen, char *serv, size_t servlen, int flags) {
  int rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);

  if (rc != 0)
    gai_error(rc, "Getnameinfo error");
}
