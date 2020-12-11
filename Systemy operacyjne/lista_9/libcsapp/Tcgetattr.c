#include "csapp.h"

void Tcgetattr(int fd, struct termios *termios_p) {
  if (tcgetattr(fd, termios_p) < 0)
    unix_error("Tcgetattr error");
}
