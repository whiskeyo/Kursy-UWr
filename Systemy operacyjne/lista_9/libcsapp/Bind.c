#include "csapp.h"

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen) {
  int rc = bind(sockfd, my_addr, addrlen);
  if (rc < 0)
    unix_error("Bind error");
}
