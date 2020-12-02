#include "csapp.h"

void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen) {
  int rc = connect(sockfd, serv_addr, addrlen);
  if (rc < 0)
    unix_error("Connect error");
}
