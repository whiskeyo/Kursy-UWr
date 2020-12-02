static char *ptrs[MAX_PTRS];
static int ptrn = 0;

static void randfill(void *ptr, size_t len) {
  char *buf = ptr;
  for (int i = 0; i < len; i++)
    buf[i] = rand();
}

static void test_alloc(void) {
  int len;
  ptrs[ptrn] = alloc_fn(&len);
  memchk_fn();
  randfill(ptrs[ptrn], len);
  memchk_fn();
  ptrn++;
}

static void test_free(void) {
  int i = rand() % ptrn;
  free_fn(ptrs[i]);
  memchk_fn();
  ptrs[i] = ptrs[--ptrn];
}

int main(void) {
  for (int i = 0; i < CYCLES; i++) {
    int n = rand() % MAX_PTRS;
    if (n > ptrn) {
      fprintf(stderr, "Allocating %d new blocks ...\n", n - ptrn);
      while (n > ptrn)
        test_alloc();
    }
    if (n < ptrn) {
      fprintf(stderr, "Freeing %d random blocks ...\n", ptrn - n);
      while (n < ptrn)
        test_free();
    }
  }
  fprintf(stderr, "Freeing %d remaining blocks ...\n", ptrn);
  while (ptrn)
    test_free();
  fprintf(stderr, "OK\n");
  return 0;
}
