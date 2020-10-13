#include "binsearch.h"

static int icmp(const void *a, const void *b) {
  int x = *(const int *)a;
  int y = *(const int *)b;
  if (x < y) return -1;
  if (x > y) return 1;
  return 0;
}

static void fill(int *arr, int n) {
  for (int i = 0; i < n; i++)
    arr[i] = fast_random() & n;

  qsort(arr, n, sizeof(int), icmp);
}

static binsearch_t binsearch[2] = {binsearch0, binsearch1};

static int run(int variant, int exp, int times, uint64_t seed) {
  printf(">>> VARIANT %d <<<\n", variant);

  printf("Random number generator seed is 0x%" PRIx64 "\n", seed);
  fast_srandom(seed);

  /* Number of elements in fully populated binary tree of height (exp - 1) */
  long n = (1 << exp) - 1;
  long size = n * sizeof(int);
  int *arr = malloc_page_aligned(size);

  printf("Generate array of 2^%d-1 elements (%ld KiB)\n", exp, size >> 10);

  fill(arr, n);
  if (variant == 1) {
    int *tmp = malloc_page_aligned(size);
    linearize(tmp, arr, n);
    free(arr);
    arr = tmp;
  }

  flush_cache();

  printf("Performing %d searches\n", times);

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  pmc_start();
  binsearch_t func = binsearch[variant];
  int found = 0;
  for (int i = 0; i < times; i++)
    found += func(arr, n, fast_random() & n);
  pmc_stop();
  timer_stop(&timer);
  timer_print(&timer);

  pmc_print();

  /* Make sure you get same results with given RNG seed! */
  printf("Number of searches that succeeded: %d/%d\n", found, times);

  free(arr);

  return found;
}

int main(int argc, char **argv) {
  pmc_evset_t evset = PMC_NONE;
  int opt, exp = -1, times = -1, variant = -1;
  bool err = false;
  bool user_seed = false;
  uint64_t seed;

  while ((opt = getopt(argc, argv, "p:n:t:v:S:")) != -1) {
    if (opt == 'p') {
      evset = pmc_evset_by_name(optarg);
    } else if (opt == 'n')
      exp = atoi(optarg);
    else if (opt == 't')
      times = 1 << atoi(optarg);
    else if (opt == 'v')
      variant = atoi(optarg);
    else if (opt == 'S')
      user_seed = true, seed = strtoul(optarg, NULL, 16);
    else
      err = true;
  }

  if (err || exp < 0 || times < 0 || variant < -1 || variant >= 2)
    die("Usage: %s -p [%s] -S hex_seed -n log2(size) "
        "-l log2(times) -v variant\n", pmc_evset_string, argv[0]);

  pmc_init(evset);

  if (!user_seed)
    read_bytes("/dev/urandom", &seed, sizeof(seed));

  if (variant == -1) {
    int expected;
    for (int i = 0; i < 2; i++) {
      if (i == 0)
        expected = run(i, exp, times, seed);
      else if (run(i, exp, times, seed) != expected)
        die("ERROR: Your optimizations yield incorrect results!\n"); 
    }
  } else {
    run(variant, exp, times, seed);
  }

  pmc_kill();

  return 0;
}
