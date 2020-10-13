#include "texture.h"

static long run(int variant, int times, uint64_t seed) {
  printf(">>> VARIANT %d <<<\n", variant);

  fast_srandom(seed);
  printf("Random number generator seed is 0x%" PRIx64 "\n", seed);

  size_t size = N * N * sizeof(T);
  T *arr = malloc_page_aligned(size);

  printf("Generate texture of %dx%d pixels (%ld KiB)\n", N, N, size >> 10);

  fill_t fill = (variant == 0) ? fill_0 : fill_1;
  query_t query = (variant == 0) ? query_0 : query_1;

  fill(arr);
  flush_cache();

  printf("Performing texture queries %d times.\n", times);

  _timer_t timer;
  timer_reset(&timer);
  timer_start(&timer);
  pmc_start();
  long r = 0, elems = 0;
  for (int i = 0; i < times; i++) {
    long a = fast_random();
    long xs = (short)(a >> 48) & (N - 1);
    long ys = (short)(a >> 32) & (N - 1);
    long xe = (short)(a >> 16) & (N - 1);
    long ye = (short)(a) & (N - 1);
    elems += query(arr, xs, ys, xe, ye, &r);
  }
  pmc_stop();
  timer_stop(&timer);
  timer_print(&timer);

  pmc_print();

  free(arr);

  printf("Scanned %ld elements. Result is %lx.\n", elems, r);

  return r;
}

int main(int argc, char **argv) {
  int opt, variant = -1, times = 0;
  pmc_evset_t evset = PMC_NONE;
  bool err = false;
  bool user_seed = false;
  uint64_t seed;

  while ((opt = getopt(argc, argv, "p:t:v:S:")) != -1) {
    if (opt == 'p') {
      evset = pmc_evset_by_name(optarg);
    } else if (opt == 't')
      times = atoi(optarg);
    else if (opt == 'v')
      variant = atoi(optarg);
    else if (opt == 'S')
      user_seed = true, seed = strtoul(optarg, NULL, 16);
    else
      err = true;
  }

  if (err || times == 0 || variant < -1 || variant >= 3)
    die("Usage: %s -p [%s] -S hex_seed  -t times -v variant\n",
        pmc_evset_string, argv[0]);

  pmc_init(evset);

  if (!user_seed)
    read_bytes("/dev/urandom", &seed, sizeof(seed));

  if (variant == -1) {
    long expected;
    for (int i = 0; i < 3; i++) {
      if (i == 0)
        expected = run(i, times, seed);
      else if (run(i, times, seed) != expected)
        die("ERROR: Your optimizations yield incorrect results!\n"); 
    }
  } else {
    run(variant, times, seed);
  }

  pmc_kill();

  return 0;
}
