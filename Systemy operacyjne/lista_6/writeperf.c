#include "csapp.h"

static noreturn void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s [-t times] [-l length] -s "
          "[write|fwrite|fwrite-line|fwrite-full|writev]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int length = -1, times = -1;
  bool dosync = false;
  int opt;

  while ((opt = getopt(argc, argv, "l:t:s")) != -1) {
    if (opt == 'l')
      length = atoi(optarg);
    else if (opt == 't')
      times = atoi(optarg);
    else if (opt == 's')
      dosync = true;
    else 
      usage(argc, argv);
  }

  if (optind >= argc || length <= 0 || times <= 0)
    usage(argc, argv);

  char *choice = argv[optind];

  char *line = malloc(length + 1);
  memset(line, '*', length);
  line[length] = '\n';

  if (strcmp(choice, "write") == 0) {
    for (int j = 0; j < times; j++)
      for (int k = 0; k < length; k++)
        write(STDOUT_FILENO, line + k, length + 1 - k); 
  }

  if (strncmp(choice, "fwrite", 6) == 0) {
    size_t size;
    int mode;
    void *buf = NULL; 

    if (strcmp(choice, "fwrite-line") == 0) {
      mode = _IOLBF;
      size = length + 1;
    } else if (strcmp(choice, "fwrite-full") == 0) {
      mode = _IOFBF;
      size = getpagesize();
    } else {
      mode = _IONBF;
      size = 0;
    }

    /* TODO: Attach new buffer to stdout stream. */
    
    setvbuf(stdout,buf,mode,size);

    for (int j = 0; j < times; j++)
      for (int k = 0; k < length; k++)
        fwrite(line + k, 1, length+1-k, stdout); 
    fflush(stdout);

    free(buf);
  }

  if (strcmp(choice, "writev") == 0) {
    int n = sysconf(_SC_IOV_MAX);
    struct iovec iov[n];
    
    /* TODO: Write file by filling in iov array and issuing writev. */
    
    for (int j = 0; j < times; j++)
      for (int k = 0; k < length; k++) {
        iov[ (j*length + k) % n ].iov_base = line + k;
        iov[ (j*length + k) % n ].iov_len = length + 1 - k;
        if((j*length + k) % n == n - 1)
            writev(STDOUT_FILENO, iov, n);
	  }
		
    if((length * times) % n)
      writev(STDOUT_FILENO, iov, (length * times) % n);
  }

  free(line);

  if (dosync && !isatty(STDOUT_FILENO))
    fsync(STDOUT_FILENO);

  return 0;
}