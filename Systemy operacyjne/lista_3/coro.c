#include "queue.h"
#include "csapp.h"

#define CORO_STKSIZE 4096
#define CORO_STKALIGN 16 /* As required by SysV ABI ! */

#ifndef EOF
#define EOF (-1)
#endif

#ifndef NOTHING
#define NOTHING (-2)
#endif

typedef struct coro {
  TAILQ_ENTRY(coro) co_link;
  const char *co_name;
  void *co_stack;
  Jmpbuf co_ctx;
} coro_t;

static TAILQ_HEAD(, coro) runqueue = TAILQ_HEAD_INITIALIZER(runqueue);
static coro_t *running;
static Jmpbuf dispatcher;

/* Initialize coroutine stucture with stack. */
static void coro_init(coro_t *co, const char *name) {
  memset(co, 0, sizeof(coro_t));
  co->co_name = name;
  /* Allocates a fresh stack for the coroutine! */
  if (posix_memalign(&co->co_stack, CORO_STKALIGN, CORO_STKSIZE) < 0)
    unix_error("posix_memalign error");
}

/* Detach a stack from coroutine structure. */
static void coro_destroy(coro_t *co) {
  free(co->co_stack);
}

/*
 * Switch between subsequent coroutines.
 *
 * Dead coroutines, i.e. ones that returned EOF, get removed from the run queue.
 * Feed next coroutine (value returned from coro_yield) with the result from
 * previous one (parameter passed to coro_yield).
 * Return to dispatcher if there're no more coroutines to run.
 */
static noreturn void coro_switch(int v) {
  coro_t *curr = running;
  
  /* TODO: Use description above to implement the body. */
  if (v == EOF) {
    TAILQ_REMOVE(&runqueue, curr, co_link);
    if (TAILQ_EMPTY(&runqueue))
      Longjmp(dispatcher, NOTHING);
  }

  if (TAILQ_NEXT(curr, co_link) == NULL) {
    running = TAILQ_FIRST(&runqueue);
    Longjmp(running->co_ctx, v);
  }

  running = TAILQ_NEXT(curr, co_link);
  Longjmp(running->co_ctx, v);
}

/* Save caller context and switch back to next coroutine. */
static int coro_yield(int v) {
  int nv = Setjmp(running->co_ctx);
  if (nv == 0)
    coro_switch(v);
  return nv;
}

/* Configure coroutine context to be executed. */
static void coro_add(coro_t *co, void (*fn)(int)) {
  int v = Setjmp(co->co_ctx);
  if (v) {
    /* This will get executed when coroutine is entered first time. */
    fn(v);
    /* Coroutine must pass EOF to be removed from runqueue! */
    coro_switch(EOF);
  }
  /* Coroutine will be running on its private stack! */
  co->co_ctx->rsp = co->co_stack + CORO_STKSIZE;
  TAILQ_INSERT_TAIL(&runqueue, co, co_link);
}

/* Take first coroutine and feed it with passed value. */
static int coro_run(int v) {
  running = TAILQ_FIRST(&runqueue);
  int nv = Setjmp(dispatcher);
  if (nv == 0)
    Longjmp(running->co_ctx, v);
  return nv;
}

/*
 * Actual coroutines that perform some useful work.
 */

static void func_1(int _) {
  int words = 0;
  char prev_ch = ' ';
  char ch;

  while (Read(0, &ch, 1) > 0) {
    if (isspace(ch)) {
      if (isspace(prev_ch))
        continue;
      words++;
    }
    coro_yield(ch);
    prev_ch = ch;
  }

  if (!isspace(ch))
    words++;

  dprintf(STDERR_FILENO, "\nfunc_1: words = %d\n", words);
}

static void func_2(int ch) {
  int removed = 0;

  while (ch != EOF) {
    if (!isalpha(ch)) {
      removed++;
      ch = NOTHING;
    }
    ch = coro_yield(ch);
  }
 
  dprintf(STDERR_FILENO, "func_2: removed = %d\n", removed);
}

static void func_3(int ch) {
  int printed = 0;

  while (ch != EOF) {
    if (ch != NOTHING) {
      printed++;
      if (islower(ch))
        ch = toupper(ch);
      else if (isupper(ch))
        ch = tolower(ch);
      Write(STDOUT_FILENO, &ch, 1);
    }
    ch = coro_yield(NOTHING);
  }

  dprintf(STDERR_FILENO, "func_3: printed = %d\n", printed);
}

int main(void) {
  coro_t co[3];

  coro_init(&co[0], "func_1");
  coro_init(&co[1], "func_2");
  coro_init(&co[2], "func_3");
  coro_add(&co[0], func_1);
  coro_add(&co[1], func_2);
  coro_add(&co[2], func_3);
  coro_run(NOTHING);
  coro_destroy(&co[0]);
  coro_destroy(&co[1]);
  coro_destroy(&co[2]);

  dprintf(STDERR_FILENO, "Bye, bye!\n");

  return EXIT_SUCCESS;
}
