#include "csapp.h"
#include "arena.h"

static arena_t *alloc_arena(void) {
  return Mmap(NULL, ARENA_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
              -1, 0);
}

arena_t *alloc_first_arena(arenalist_t *al) {
  arena_t *newar = alloc_arena();
  STAILQ_INSERT_HEAD(al, newar, arenalink);
  return newar;
}

arena_t *alloc_after_arena(arenalist_t *al, arena_t *ar) {
  arena_t *newar = alloc_arena();
  if (ar == NULL)
    STAILQ_INSERT_TAIL(al, newar, arenalink);
  else
    STAILQ_INSERT_AFTER(al, ar, newar, arenalink);
  return newar;
}

arena_t *find_ptr_arena(const arenalist_t *al, const void *ptr) {
  arena_t *ar;
  STAILQ_FOREACH(ar, al, arenalink) {
    if ((intptr_t)ptr > (intptr_t)ar &&
        (intptr_t)ptr < (intptr_t)ar + ARENA_SIZE) {
      return ar;
    }
  }
  return NULL;
}
