#include "randwalk.h"

static void fill(uint8_t *dst, int n) {
  for (int i = 0; i < n * n; i++)
    dst[i] = fast_random();
}

static randwalk_t randwalk[2] = {randwalk0, randwalk1};

static int run(int variant, int size, int steps, int times, uint64_t seed) {
  printf(">>> VARIANT %d <<<\n", variant);

  fast_srandom(seed);
  printf("Random number generator seed is 0x%" PRIx64 "\n", seed);

  uint8_t *array = malloc_page_aligned(size * size);

  printf("Generate matrix %d x %d (%d KiB)\n", size, size, size * size >> 10);

  fill(array, size);
  flush_cache();

  printf("Performing %d random walks of %d steps.\n", times, steps);

  int sum = 0;
  randwalk_t func = randwalk[variant];

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  for (int i = 0; i < times; i++) {
    pmc_start();
    sum += func(array, size, steps);
    pmc_stop();
  }

  pmc_print();
  //return sum;
  timer_stop(&timer);
  timer_print(&timer);

  /* Make sure you get same results with given RNG seed! */
  printf("Walks accrued elements worth: %u\n", sum);

  free(array);

  return sum;
}

int main(int argc, char **argv) {
  int opt, size = -1, steps = -1, times = -1, variant = -1;
  pmc_evset_t evset = PMC_NONE;
  bool err = false;
  bool user_seed = false;
  uint64_t seed;

  while ((opt = getopt(argc, argv, "p:n:s:t:v:S:")) != -1) {
    if (opt == 'p') {
      evset = pmc_evset_by_name(optarg);
    } else if (opt == 'n')
      size = 1 << atoi(optarg);
    else if (opt == 's')
      steps = 1 << atoi(optarg);
    else if (opt == 't')
      times = 1 << atoi(optarg);
    else if (opt == 'v')
      variant = atoi(optarg);
    else if (opt == 'S')
      user_seed = true, seed = strtoul(optarg, NULL, 16);
    else
      err = true;
  }

  if (err || size < 0 || steps < 0 || times < 0 || variant < -1 || variant >= 2)
    die("Usage: %s -p [%s] -S hex_seed -n log2(size) -s log2(steps) "
        "-t log2(times) -v variant\n", pmc_evset_string, argv[0]);

  pmc_init(evset);

  if (!user_seed)
    read_bytes("/dev/urandom", &seed, sizeof(seed));

  if (variant == -1) {
    int expected;
    for (int i = 0; i < 2; i++) {
      if (i == 0)
        expected = run(i, size, steps, times, seed);
      else if (run(i, size, steps, times, seed) != expected)
        die("ERROR: Your optimizations yield incorrect results!\n"); 
    }
  } else {
    run(variant, size, steps, times, seed);
  }

  pmc_kill();

  return EXIT_SUCCESS;
}
