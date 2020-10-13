#ifndef __COMMON_H__
#define __COMMON_H__

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include <x86intrin.h>

#define CONCAT2(A, B) A ## B
#define CONCAT(A, B) CONCAT2(A, B)

/* Block size for Intel® x86-64. */
#define BLOCK_SIZE 64
/* Hopefully no one has more than 16MiB of L3 cache. */
#define CACHE_SIZE (1 << 24)

/* Number of items of type T in array of size S. */
#define NITEMS(S, T) ((S) / sizeof(T))
/* Function attribute that prevents compiler from inlining it. */
#ifndef __noinline
#define __noinline __attribute__ ((noinline))
#endif
/* Function argument attribute to mark unused argument. */
#ifndef __unused
#define __unused __attribute__ ((unused))
#endif
/* Makes function being called automatically before main(). */
#ifndef __constructor
#define __constructor __attribute__((constructor)) 
#endif
/* Prevents _compiler_ from reordering memory read & write operations. */
#define barrier() asm volatile("" ::: "memory")
/* Prevents _hardware_ from reordering memory read & write operations. */
#define mfence() asm volatile("mfence")

/* Tell branch predictor that following condition is likely / unlikely
 * to be satisfied. */
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

typedef struct _timer {
  struct timespec start, sum;
} _timer_t;

void die(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

void fast_srandom(uint64_t seed);
uint64_t fast_random();

void flush_cache();

void timer_reset(_timer_t *t);
void timer_start(_timer_t *t);
void timer_stop(_timer_t *t);
void timer_print(_timer_t *t);

void read_bytes(const char *pathname, void *buf, size_t n);
void *malloc_page_aligned(size_t size);

/* Performance Measurement Counter wrapper API. */
#ifndef PAPI
#define PAPI 0
#endif

typedef enum {
  PMC_NONE, PMC_IPC, PMC_BRANCH, PMC_MEMORY, PMC_L1, PMC_L2, PMC_L3, PMC_TLB
} pmc_evset_t;

pmc_evset_t pmc_evset_by_name(const char *name);
void pmc_init(pmc_evset_t evset);
void pmc_kill(void);
void pmc_start(void);
void pmc_stop(void);
void pmc_print(void);

extern const char pmc_evset_string[];

#endif
