#include "common.h"
#include "matmult.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

static void fill(T_p dst, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      dst[i * n + j] = min(i + 1, j + 1);
}

static matmult_t matmult[4] = {matmult0, matmult1, matmult2, matmult3};

int main(int argc, char **argv) {
  int opt, variant = -1;
  long n = 0;
  pmc_evset_t evset = PMC_NONE;
  bool err = false;

  while ((opt = getopt(argc, argv, "p:n:v:")) != -1) {
    if (opt == 'p') {
      if (strcmp(optarg, "ipc") == 0)
        evset = PMC_IPC;
      else if (strcmp(optarg, "branch") == 0)
        evset = PMC_BRANCH;
      else if (strcmp(optarg, "memory") == 0)
        evset = PMC_MEMORY;
    } else if (opt == 'n')
      n = atoi(optarg);
    else if (opt == 'v')
      variant = atoi(optarg);
    else
      err = true;
  }

  if (err || n == 0 || variant < 0 || variant >= 4)
    die("Usage: %s -p [ipc|branch|memory] -n size -v variant\n", argv[0]);

  pmc_init(evset);

  if (n % BLOCK)
    die("Matrix size (%ld) must be divisible by %d!\n", n, BLOCK);

  size_t size = n * n * sizeof(T);
  size_t pagesize = getpagesize();

  /* Allocate space for each matrix plus extra space (of page size). */
  T *a = malloc_page_aligned(size + pagesize);
  T *b = malloc_page_aligned(size + pagesize);
  T *c = malloc_page_aligned(size + pagesize);

  printf("Generate 2 matrices %ld x %ld (%ld KiB each)\n", n, n, size >> 10);

  T_p ma = a + A_OFFSET;
  T_p mb = b + B_OFFSET;
  T_p mc = c + C_OFFSET;

  fill(ma, n);
  fill(mb, n);
  bzero(mc, size);
  flush_cache();

  printf("Performing matrix multiplication.\n");

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  pmc_start();
  matmult[variant](n, ma, mb, mc);
  pmc_stop();
  timer_stop(&timer);
  timer_print(&timer);

  pmc_print();

  /* Rudimentary check. We don't rely on it! */
  T expected = n * (n + 1) * (2 * n + 1) / 6;
  T value = mc[n * n - 1];
  if (value != expected)
    die("Incorrect answer: %ld (expected: %ld)!", value, expected);

  free(a);
  free(b);
  free(c);

  pmc_kill();

  return 0;
}
