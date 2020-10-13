#include "transpose.h"

static void fill(T *dst, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      dst[i * n + j] = i * n + j;
}

static transpose_t transpose[] = {transpose0, transpose1};

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

  if (err || n == 0 || variant < 0 || variant >= 2)
    die("Usage: %s -p [ipc|branch|memory] -n size -v variant\n", argv[0]);

  pmc_init(evset);

  if (n % BLOCK)
    die("Matrix size (%ld) must be divisible by %d!", n, BLOCK);

  size_t size = n * n * sizeof(T);
  T *src = malloc_page_aligned(size);
  T *dst = malloc_page_aligned(size);

  printf("Generate matrix %ld x %ld (%ld KiB)\n", n, n, size >> 10);

  fill(src, n);
  bzero(dst, size);
  flush_cache();

  printf("Performing matrix transposition.\n");

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  pmc_start();
  transpose[variant](dst, src, n);
  pmc_stop();
  timer_stop(&timer);
  timer_print(&timer);

  pmc_print();

  /* Rudimentary check. We don't rely on it! */
  T expected = n * (n - 1);
  T value = dst[n - 1];
  if (value != expected)
    die("Incorrect answer: %d (expected: %d)!", value, expected);


  free(src);
  free(dst);

  pmc_kill();

  return 0;
}
