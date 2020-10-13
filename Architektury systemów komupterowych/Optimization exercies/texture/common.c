#define _GNU_SOURCE
#include <stdarg.h>
#include <sched.h>
#include <sys/mman.h>
#include "common.h"

void die(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}

/* The state must be seeded with a nonzero value. */
static uint64_t __state;

/* This procedure gets called before main(). */
__constructor void __init_fast_random(void) {
  __state = time(NULL);
}

void fast_srandom(uint64_t seed) {
  __state = seed;
}

uint64_t fast_random(void) {
  __state ^= __state >> 12;
  __state ^= __state << 25;
  __state ^= __state >> 27;
  return __state * 2685821657736338717UL;
}

__constructor void __init_cpu(void) {
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);
  if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
    die("Error: Failed to set process CPU affinity!\n");
}

void flush_cache() {
  void *tmp = malloc_page_aligned(CACHE_SIZE);
  bzero(tmp, CACHE_SIZE);
  free(tmp);
}

typedef struct timespec timespec_t;

#define SECOND 1000000000 /* in nanoseconds */

static void timespec_add(timespec_t *a, timespec_t *b, timespec_t *res) {
  if (a->tv_nsec + b->tv_nsec >= SECOND) {
    res->tv_sec = a->tv_sec + b->tv_sec + 1;
    res->tv_nsec = a->tv_nsec + b->tv_nsec - SECOND;
  } else {
    res->tv_sec = a->tv_sec + b->tv_sec;
    res->tv_nsec = a->tv_nsec + b->tv_nsec;
  }
}

static void timespec_sub(timespec_t *a, timespec_t *b, timespec_t *res) {
  if (a->tv_nsec - b->tv_nsec < 0) {
    res->tv_sec = a->tv_sec - b->tv_sec - 1;
    res->tv_nsec = a->tv_nsec - b->tv_nsec + SECOND;
  } else {
    res->tv_sec = a->tv_sec - b->tv_sec;
    res->tv_nsec = a->tv_nsec - b->tv_nsec;
  }
}

void timer_reset(_timer_t *t) {
  memset(t, 0, sizeof(_timer_t));
}

void timer_start(_timer_t *t) {
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t->start);
}

void timer_stop(_timer_t *t) {
  timespec_t ts_end, ts_res;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_end);
  timespec_sub(&ts_end, &t->start, &ts_res);
  timespec_add(&t->sum, &ts_res, &t->sum);
}

void timer_print(_timer_t *t) {
  printf("Time elapsed: %ld.%06ld seconds\n",
         t->sum.tv_sec, t->sum.tv_nsec / 1000);
}

void read_bytes(const char *pathname, void *buf, size_t n) {
  int fd = open(pathname, 0);
  read(fd, buf, n);
  close(fd);
}

void *malloc_page_aligned(size_t size) {
  void *ptr = NULL;
  posix_memalign((void **)&ptr, getpagesize(), size);
  if (ptr == NULL)
    die("Error: Cannot allocate %ld bytes!\n", size);
  // madvise(ptr, size, MADV_HUGEPAGE);
  return ptr;
}

#if PAPI
#include <papi.h>

/*
 * Based on /usr/share/papi/5.7/papi_events.csv
 *
 * PAPI_TOT_INS: total instructions
 * PAPI_TOT_CYC: total cycles
 * PAPI_BR_CN: conditional branch instructions
 * PAPI_BR_MSP: mispredicted branches
 * PAPI_LD_INS: retired load instructions
 * PAPI_SR_INS: retired store instructions
 * PAPI_L1_DCM: L1 Data Cache misses
 * PAPI_L2_DCA: L2 Data Cache accesses
 * PAPI_L2_DCM: L2 Data Cache misses
 * PAPI_TLB_DM: data TLB misses
 */

#define MAX_EVENT 10
#define MIN_HWCTRS 4

static int eventset = PAPI_NULL;
static long long eventcount[MAX_EVENT];

typedef struct evset {
  const char *name;
  long idx;
} evset_t;

static evset_t empty_evset[] = {
  { NULL, -1 }
};

static evset_t ipc_evset[] = {
  { "PAPI_TOT_INS", -1 },
  { "PAPI_TOT_CYC", -1 },
  { NULL, - 1 }
};

static evset_t branch_evset[] = {
  { "PAPI_BR_CN", -1 },
  { "PAPI_BR_MSP", -1 },
  { NULL, -1 }
};

static evset_t memory_evset[] = {
  { "PAPI_LD_INS", -1 },
  { "PAPI_SR_INS", -1 },
  { "PAPI_L1_DCM", -1 },
  { "PAPI_L2_DCM", -1 },
  { "PAPI_L3_TCM", -1 },
  { "PAPI_TLB_DM", -1 },
  { NULL, -1 },
};

static evset_t l1cache_evset[] = {
  { "PAPI_LD_INS", -1 },
  { "PAPI_SR_INS", -1 },
  { "PAPI_L1_DCM", -1 },
  { NULL, -1 },
};

static evset_t l2cache_evset[] = {
  { "PAPI_LD_INS", -1 },
  { "PAPI_SR_INS", -1 },
  { "PAPI_L2_DCM", -1 },
  { NULL, -1 },
};

static evset_t l3cache_evset[] = {
  { "PAPI_LD_INS", -1 },
  { "PAPI_SR_INS", -1 },
  { "PAPI_L3_TCM", -1 },
  { NULL, -1 },
};

static evset_t tlb_evset[] = {
  { "PAPI_LD_INS", -1 },
  { "PAPI_SR_INS", -1 },
  { "PAPI_TLB_DM", -1 },
  { NULL, -1 },
};

static evset_t *all_evset[] = {
  empty_evset, ipc_evset, branch_evset, memory_evset,
  l1cache_evset, l2cache_evset, l3cache_evset, tlb_evset
};

static evset_t *cur_evset = NULL;
#endif

pmc_evset_t pmc_evset_by_name(const char *name) {
  if (strcmp(name, "ipc") == 0)
    return PMC_IPC;
  if (strcmp(name, "branch") == 0)
    return PMC_BRANCH;
  if (strcmp(name, "memory") == 0)
    return PMC_MEMORY;
  if (strcmp(name, "l1") == 0)
    return PMC_L1;
  if (strcmp(name, "l2") == 0)
    return PMC_L2;
  if (strcmp(name, "l3") == 0)
    return PMC_L3;
  if (strcmp(name, "tlb") == 0)
    return PMC_TLB;
  return PMC_NONE;
}

const char pmc_evset_string[] = "ipc|branch|memory|l1|l2|l3|tlb";

void pmc_init(pmc_evset_t evset) {
#if PAPI
  int retval;

	retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT)
		die("PAPI_library_init failed: %s!\n", PAPI_strerror(retval));

  const PAPI_hw_info_t *hwinfo = PAPI_get_hardware_info();
  if (hwinfo == NULL)
    die("Could not fetch PAPI hardware information!\n");
  if (hwinfo->virtualized)
    die("Running inside virtual machine is not supported!\n");

  retval = PAPI_get_opt(PAPI_MAX_HWCTRS, NULL);
  if (retval < 0)
    die("Number of hardware counters: %s\n", PAPI_strerror(retval));
  if (retval == 0)
    die("No hardware counters available! Maybe they are disabled for user?\n"
        "Try to run 'sysctl kernel.perf_event_paranoid=-1' as root.\n");
  if (retval < MIN_HWCTRS)
    die("Configuration not supported: "
        "need at least %d hardware counters, got only %d.\n"
        "Try disabling hyper-threading in BIOS settings if possible!\n",
        MIN_HWCTRS, retval);

	retval = PAPI_create_eventset(&eventset);
	if (retval != PAPI_OK)
		die("PAPI_create_eventset failed: %s!\n", PAPI_strerror(retval));

  cur_evset = all_evset[evset];

  int idx = 0;
  for (evset_t *es = cur_evset; es->name; es++) {
    retval = PAPI_add_named_event(eventset, es->name);
    if (retval != PAPI_OK)
      die("PAPI_add_named_event %s: %s\n"
          "Report output from 'papi_component_avail' command!\n",
          es->name, PAPI_strerror(retval));
    es->idx = idx++;
  }

  PAPI_reset(eventset);

  memset(eventcount, 0, sizeof(eventcount));
#endif
}

void pmc_kill(void) {
#if PAPI
  PAPI_shutdown();
#endif
}

void pmc_start(void) {
#if PAPI
  PAPI_start(eventset);
#endif
}

void pmc_stop(void) {
#if PAPI
  if (cur_evset == empty_evset)
    return;
  long long count[MAX_EVENT];
  int retval = PAPI_stop(eventset, count);
  if (retval != PAPI_OK)
    die("PAPI_stop failed with %s!\n", PAPI_strerror(retval));
  for (int i = 0; i < MAX_EVENT; i++)
    eventcount[i] += count[i];
#endif
}

static long long pmc_read(const char *name) {
#if PAPI
  for (evset_t *es = cur_evset; es->name; es++)
    if (strcmp(es->name, name) == 0)
      return eventcount[es->idx];
#endif
  return 0;
}

void pmc_print(void) {
#if PAPI
  if (cur_evset == ipc_evset) {
    printf("> Total instructions: %ld\n", (long)pmc_read("PAPI_TOT_INS"));
    printf("> Instructions per cycle: %2.3f\n",
           (double)pmc_read("PAPI_TOT_INS") / (double)pmc_read("PAPI_TOT_CYC"));
  } else if (cur_evset == branch_evset) {
    printf("> Branch misprediction ratio: %2.3f%%\n",
           100.0 * (double)pmc_read("PAPI_BR_MSP") /
           (double)pmc_read("PAPI_BR_CN"));
  } else if (cur_evset != empty_evset) {
    double ldst_insns = pmc_read("PAPI_LD_INS") + pmc_read("PAPI_SR_INS");
    if (cur_evset == memory_evset || cur_evset == l1cache_evset)
      printf("> L1 Data Cache miss ratio: %2.3f%%\n",
             100.0 * pmc_read("PAPI_L1_DCM")/ldst_insns);
    if (cur_evset == memory_evset || cur_evset == l2cache_evset)
      printf("> L2 Data Cache miss ratio: %2.3f%%\n",
             100.0 * pmc_read("PAPI_L2_DCM")/ldst_insns);
    if (cur_evset == memory_evset || cur_evset == l3cache_evset)
      printf("> L3 Cache miss ratio: %2.3f%%\n",
             100.0 * pmc_read("PAPI_L3_TCM")/ldst_insns);
    if (cur_evset == memory_evset || cur_evset == tlb_evset)
      printf("> Data TLB miss ratio: %2.3f%%\n",
             100.0 * pmc_read("PAPI_TLB_DM")/ldst_insns);
  }
#endif
}
