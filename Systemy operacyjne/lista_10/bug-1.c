/* WARNING: This code is buggy! */
#include "csapp.h"

/* Global shared variable */
// static volatile long cnt = 0; /* Counter */
static _Atomic long cnt = 0; // fixes bug-1

/* Thread routine */
static void *thread(void *vargp) {
  long i, niters = *((long *)vargp);

  for (i = 0; i < niters; i++)
    cnt++;

  return NULL;
}

int main(int argc, char **argv) {
  /* Check input argument */
  if (argc != 2)
    app_error("usage: %s <niters>\n", argv[0]);

  long niters = atoi(argv[1]);
  pthread_t tid1, tid2;

  /* Create threads and wait for them to finish */
  Pthread_create(&tid1, NULL, thread, &niters);
  Pthread_create(&tid2, NULL, thread, &niters);
  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);

  /* Check result */
  if (cnt != (2 * niters)) {
    printf("BOOM! cnt=%ld\n", cnt);
    return EXIT_FAILURE;
  }

  printf("OK cnt=%ld\n", cnt);
  return EXIT_SUCCESS;
}
