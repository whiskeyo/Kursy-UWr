#ifndef _ARENA_H_
#define _ARENA_H_

#include "queue.h"

/* Default size of an arena. */
#ifndef ARENA_SIZE
#define ARENA_SIZE (1 << 16)
#endif

/* Extra data stored in arena header by a memory manager. */
#ifndef ARENA_EXTRA
#define ARENA_EXTRA struct {}
#endif

typedef struct arena {
  STAILQ_ENTRY(arena) arenalink; /* link on list of all arenas */
  ARENA_EXTRA;
} arena_t;

typedef STAILQ_HEAD(arenalink, arena) arenalist_t;

/* Allocate first arena if arena list is empty. */
arena_t *alloc_first_arena(arenalist_t *al);
/* Allocate arena and insert it into list.
 * If ar is not NULL insert after ar, otherwise append to the list. */
arena_t *alloc_after_arena(arenalist_t *al, arena_t *ar);
/* Find an arena on list that ptr belongs to, otherwise return NULL. */
arena_t *find_ptr_arena(const arenalist_t *al, const void *ptr);
/* Calculate address of place where arena ends. */
static inline void *arena_end(arena_t *ar) {
  return (void *)ar + ARENA_SIZE;
}

#endif /* !_ARENA_H_ */
