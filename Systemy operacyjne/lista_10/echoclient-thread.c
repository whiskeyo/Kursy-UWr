#include "csapp.h"
#include "rio.h"

#define THREADMAX 100
#define ITERMAX 50

/* Read whole file into process address space.
 * Make sure it ends with a newline and NUL character. */
static char *readtext(const char *path) {
  int fd = Open(path, O_RDONLY, 0);

  struct stat sb;
  Fstat(fd, &sb);

  size_t len = sb.st_size;
  char *data = Malloc(len + 2);
  Read(fd, data, len);

  Close(fd);

  if (data[len - 1] != '\n')
    data[len++] = '\n';
  data[len] = '\0';

  return data;
}

typedef struct line {
  const char *data;
  size_t length;
} line_t;

static const size_t splitlines(const char *text, line_t **linesp) {
  size_t capacity = 10;
  size_t nlines;

  line_t *lines = Malloc(capacity * sizeof(line_t));
  
  for (nlines = 0; *text; nlines++) {
    /* Grow lines array if needed. */
    if (nlines == capacity) {
      capacity *= 2;
      lines = Realloc(lines, capacity * sizeof(line_t));
    }

    const char *begin = text;

    /* Advance to the next line. */
    while (*text && *text != '\n')
      text++;

    /* Skip newline character. */
    if (*text == '\n')
      text++;

    lines[nlines].data = begin;
    lines[nlines].length = text - begin;
  }

  /* Trim lines array down. */
  *linesp = Realloc(lines, nlines * sizeof(line_t));
  return nlines;
}

static sig_atomic_t quit = false;

static void sigint_handler(int sig) {
  safe_printf("Quit requested!\n");
  quit = true;
}

static char *c_host = NULL;
static char *c_port = NULL;
static line_t *c_lines = NULL;
size_t c_nlines = 0;

static void *thread(void *data) {
  unsigned seed = (unsigned)pthread_self();
  char *buf = NULL;
  size_t buflen = 0;

  while (!quit) {
    int fd = Open_clientfd(c_host, c_port);

    for (int i = 0; i < ITERMAX && !quit; i++) {
      line_t *line = &c_lines[rand_r(&seed) % c_nlines];

      if (buflen < line->length) {
        buflen = line->length;
        buf = Realloc(buf, buflen);
      }

      Rio_writen(fd, (void *)line->data, line->length);
      Rio_readn(fd, buf, line->length);
      assert(memcmp(buf, line->data, line->length) == 0);
      usleep(1000 * (rand_r(&seed) % 50));
    }

    Close(fd);
  }

  free(buf);
  return NULL;
}

int main(int argc, char **argv) {
  if (argc != 5)
    app_error("usage: %s <textfile> <nthreads> <host> <port>\n", argv[0]);

  size_t nthreads = atol(argv[2]);
  assert(nthreads > 0 && nthreads <= THREADMAX);

  char *text = readtext(argv[1]);

  c_nlines = splitlines(text, &c_lines);
  c_host = argv[3];
  c_port = argv[4];

  Signal(SIGINT, sigint_handler);

  /* TODO: Start threads and wait for them to finish. */
  pthread_t tid[nthreads];
  for (int i = 0; i < nthreads; i++)
    Pthread_create(&tid[i], NULL, thread, NULL);

  for (int i = 0; i < nthreads; i++)
    Pthread_join(tid[i], NULL);

  free(text);
}
