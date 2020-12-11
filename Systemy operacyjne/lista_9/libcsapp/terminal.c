#include <termios.h>
#include <sys/ioctl.h>

#include "csapp.h"
#include "terminal.h"

int tty_open(void) {
  const char *dev = ttyname(STDIN_FILENO);
  if (!dev) {
    errno = ENOTTY;
    unix_error("tty_open error");
  }
  return Open(dev, O_RDWR | O_NOCTTY, 0);
}

void tty_curpos(int fd, int *x, int *y) {
  struct termios ts, ots;

  tcgetattr(fd, &ts);
  memcpy(&ots, &ts, sizeof(struct termios));
  ts.c_lflag &= ~(ECHO | ICANON | CREAD);
  tcsetattr(fd, TCSADRAIN, &ts);

  /* How many characters in the input queue. */
  int m = 0;
  /* TODO: Need to figure out some other way to do it on MacOS / FreeBSD. */
#ifdef LINUX
  ioctl(fd, TIOCINQ, &m);
#endif

  /* Read them all. */
  char discarded[m];
  m = Read(fd, discarded, m);

  Write(fd, CPR(), sizeof(CPR()));
  char buf[20];
  int n = Read(fd, buf, 19);
  buf[n] = '\0';

  ts.c_lflag |= ICANON;
  tcsetattr(fd, TCSADRAIN, &ts);
  for (int i = 0; i < m; i++)
    ioctl(fd, TIOCSTI, discarded + i);

  tcsetattr(fd, TCSADRAIN, &ots);
  sscanf(buf, "\033[%d;%dR", x, y);
}
