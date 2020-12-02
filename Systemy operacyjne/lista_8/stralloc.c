#include "csapp.h"
#include "arena.h"

#define MAX_LENGTH 126
#define MAX_BLKSZ (MAX_LENGTH + 1)

#define DEBUG_LEVEL 2

static arenalist_t arenas = STAILQ_HEAD_INITIALIZER(arenas);
static arena_t *cur_arena = NULL;
static unsigned cur_offset;

static void init_chunk(void *ar) {
  uint8_t *data = ar;
  unsigned offset = sizeof(arena_t);

  while (offset + MAX_BLKSZ < ARENA_SIZE) {
    data[offset] = -MAX_BLKSZ;
    offset += MAX_BLKSZ;
  }

  data[offset] = -(ARENA_SIZE - 1 - offset);
  data[ARENA_SIZE - 1] = 0;
}

#define BLOCK_HEADER(data) (data[cur_offset])
#define END_OF_CHUNK(data) (BLOCK_HEADER(data) == 0)
#define BLOCK_USED(data) (BLOCK_HEADER(data) > 0)
#define BLOCK_SIZE(data) (abs(BLOCK_HEADER(data)))

#define NEXT_BLOCK_HEADER(data) (data[cur_offset + BLOCK_SIZE(data)])
#define NEXT_BLOCK_FREE(data) (NEXT_BLOCK_HEADER(data) < 0)
#define NEXT_BLOCK_SIZE(data) (abs(NEXT_BLOCK_HEADER(data)))

#define MOVE_NEXT(data) cur_offset += BLOCK_SIZE(data)
#define CURRENT_PTR(data) (data + cur_offset + 1)

static void *alloc_block(int8_t *data, uint8_t len) {
  void *result = NULL;

  while (!END_OF_CHUNK(data)) {
    if (BLOCK_USED(data)) {
      /* TODO: occupied block */
      MOVE_NEXT(data);
    } else if (BLOCK_SIZE(data) == len) {
      /* TODO: free block of exact size */
      BLOCK_HEADER(data) = BLOCK_SIZE(data);
      result = CURRENT_PTR(data);
      break;
    } else if (BLOCK_SIZE(data) > len) {
      /* TODO: free block is too large */
      int8_t free_space = BLOCK_SIZE(data) - len;
      BLOCK_HEADER(data) = len;
      NEXT_BLOCK_HEADER(data) = free_space;
      result = CURRENT_PTR(data);
      break;
    } else if (!NEXT_BLOCK_FREE(data)) {
      /* TODO: next block is occupied or does not exists */
      MOVE_NEXT(data);
    } else if (NEXT_BLOCK_SIZE(data) <= len - BLOCK_SIZE(data)) {
      /* TODO: merge two free blocks, but do not allocate */
      int8_t size = BLOCK_SIZE(data) + NEXT_BLOCK_SIZE(data);
      BLOCK_HEADER(data) = -size;
    } else {
      /* TODO: merge two free blocks and allocate with split */
      int8_t total_size = BLOCK_SIZE(data) + NEXT_BLOCK_SIZE(data);
      int8_t next_block_size = total_size - len;
      BLOCK_HEADER(data) = len;
      NEXT_BLOCK_HEADER(data) = -next_block_size;
      result = CURRENT_PTR(data);
      break;
    }
  }
  return result;
}

static char *stralloc(size_t len) {
  assert(len <= MAX_LENGTH);

  if (len == 0)
    return NULL;

  if (cur_arena == NULL) {
    /* there are no arenas yet */
    init_chunk(alloc_first_arena(&arenas));
    cur_arena = STAILQ_FIRST(&arenas);
    cur_offset = sizeof(arena_t);
  }

  arena_t *ar = cur_arena;
  char *ptr;

  while (!(ptr = alloc_block((int8_t *)ar, len + 1))) {
    /* no space left on the current arena, move to the next one */
    ar = STAILQ_NEXT(ar, arenalink);
    cur_offset = sizeof(arena_t);

    /* that was the last arena, move to the first one */
    if (ar == NULL)
      ar = STAILQ_FIRST(&arenas);

    if (ar == cur_arena) { /* we've checked all arenas on the list. */
      init_chunk(alloc_after_arena(&arenas, ar));
      ar = STAILQ_NEXT(ar, arenalink);
    }
  }

  cur_arena = ar;
  return ptr;
}

static void strfree(char *str) {
  if (str == NULL)
    return;
  int8_t *sstr = (int8_t *)str;
#if DEBUG_LEVEL > 0
  assert(sstr[-1] > 0);
  arena_t *ar = find_ptr_arena(&arenas, str);
  assert(ar != NULL);
#if DEBUG_LEVEL > 1
  int8_t *ptr = (int8_t *)ar + sizeof(arena_t);
  while (ptr < sstr - 1) {
    assert(*ptr != 0);
    ptr += (*ptr > 0 ? *ptr : -*ptr);
  }
  assert(ptr == sstr - 1);
#endif
#endif
  /* TODO: mark block as free */
  assert (*ptr > 0);
  *ptr *= -1;
}

static void strmemcheck(void) {
  arena_t *ar;
  STAILQ_FOREACH(ar, &arenas, arenalink) {
    const int8_t *data = (const int8_t *)ar;
    unsigned offset = sizeof(arena_t);
    while (offset != ARENA_SIZE - 1) {
      assert(offset < ARENA_SIZE);
      assert(data[offset] != 0);
      offset += abs(data[offset]);
    }
    assert(data[offset] == 0);
  }
}

/* The test */

#define MAX_PTRS 10000
#define MAX_LEN 100
#define CYCLES 100

static void *alloc_fn(int *lenp) {
  *lenp = rand() % (MAX_LEN + 1);
  return stralloc(*lenp);
}

#define free_fn(ptr) strfree(ptr)
#define memchk_fn() strmemcheck()

#include "test.h"
