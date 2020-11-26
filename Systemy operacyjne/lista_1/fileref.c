#include "csapp.h"

static char buf[256];

#define LINE1 49
#define LINE2 33
#define LINE3 78

static void do_read(int fd) {
  /* TODO: Spawn a child. Read from the file descriptor in both parent and
   * child. Check how file cursor value has changed in both processes. */
  pid_t pid = Fork();
  if (pid == 0) {
    Read(fd, buf, 16);
    printf("%d: %ld\n", pid, Lseek(fd, 0, SEEK_CUR));
  } else {
    Read(fd, buf, 16);
    printf("%d: %ld\n", pid, Lseek(fd, 0, SEEK_CUR));
  }

  exit(0);
}

static void do_close(int fd) {
  /* TODO: In the child close file descriptor, in the parent wait for child to
   * die and check if the file descriptor is still accessible. */
  int child_status;
  pid_t pid = Fork();
  if (pid == 0) {
    printf("%d: Closing file %d\n", pid, fd);
    Close(fd);
    exit(0);
  } else {
    Waitpid(pid, &child_status, 0);
    printf("%d: Child status: %d\n", pid, child_status);
    printf("%d: Cursor position: %ld\n", pid, Lseek(fd, 0, SEEK_CUR));
    Read(fd, buf, 16);
    printf("%d: Reading from file %d: %s\n", pid, fd, buf);
    exit(0);
  }

  exit(0);
}

int main(int argc, char **argv) {
  if (argc != 2)
    app_error("Usage: %s [read|close]", argv[0]);

  int fd = Open("test.txt", O_RDONLY, 0);

  if (!strcmp(argv[1], "read"))
    do_read(fd);
  if (!strcmp(argv[1], "close"))
    do_close(fd);
  app_error("Unknown variant '%s'", argv[1]);
}
