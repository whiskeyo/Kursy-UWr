#include "csapp.h"

#define BLKSZ 4096 /* block size */
#define NBLKS 8192 /* number of blocks written to a file */
#define WRAP 64

int main(void) {
  int fd = Open("holes.bin", O_CREAT | O_TRUNC | O_WRONLY, 0644);

  int usedblks = 0;

  for (int i = 0; i < NBLKS; i++) {
    if (i % WRAP == 0)
      dprintf(STDERR_FILENO, "%04d ", i);
    if (random() % 64) {
      Lseek(fd, BLKSZ, SEEK_CUR);
      Write(STDERR_FILENO, ".", 1);
    } else {
      char blk[BLKSZ];
      for (int j = 0; j < BLKSZ; j++)
        blk[j] = random();
      Write(fd, blk, BLKSZ);
      Write(STDERR_FILENO, "O", 1);
      usedblks++;
    }
    if (i % WRAP == WRAP - 1)
      Write(STDERR_FILENO, "\n", 1);
  }
  Close(fd);

  dprintf(STDERR_FILENO, "Non-zero blocks: %d\n", usedblks);

  return 0;
}
