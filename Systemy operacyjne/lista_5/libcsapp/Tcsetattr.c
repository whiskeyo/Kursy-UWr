#include "csapp.h"

void Tcsetattr(int fd, int action, const struct termios *termios_p) {
  if (tcsetattr(fd, action, termios_p) < 0)
    unix_error("Tcsetattr error");
}
