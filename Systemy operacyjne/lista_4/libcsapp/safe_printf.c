#include <stdarg.h>
#include "csapp.h"

/* This is actually used with radix [2..36] */
static char const digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

#define MAXNBUF (sizeof(intmax_t) * 8 + 1)
#define LINELEN 512

static char *print_num(char *nbuf, uintmax_t num, int base, int *len_p) {
  char *p = nbuf;

  *p = '\0';
  do {
    *++p = digits[num % base];
  } while (num /= base);

  *len_p = p - nbuf;
  return p;
}

static int safe_vprintf(int fd, const char *fmt, va_list ap) {
  char line[LINELEN];
  char nbuf[MAXNBUF];
  int linelen = 0;
  int stop = 0;

#define PCHAR(c)                                                               \
  {                                                                            \
    int _c = (c);                                                              \
    if (linelen < MAXLINE)                                                     \
      line[linelen++] = _c;                                                    \
  }

  if (fmt == NULL)
    fmt = "(null)\n";

  for (;;) {
    int neg = 0, lflag = 0;
    int ch, n, base, sign;
    uintmax_t num;

    while ((ch = *fmt++) != '%' || stop) {
      if (ch == '\0')
        goto print;
      PCHAR(ch);
    }

    const char *percent = fmt - 1;
    char *p;

  again:
    switch ((ch = *fmt++)) {
      case '%':
        PCHAR(ch);
        break;

      case 'l':
        lflag = 1;
        goto again;

      case 'c':
        PCHAR(va_arg(ap, int));
        break;

      case 's':
        p = va_arg(ap, char *);
        if (p == NULL)
          p = "(null)";
        n = strlen(p);
        while (n--)
          PCHAR(*p++);
        break;

      case 'd':
        base = 10;
        sign = 1;
        if (lflag)
          num = va_arg(ap, long);
        else
          num = va_arg(ap, int);
        goto number;

      case 'x':
        base = 16;
        sign = 0;
        if (lflag)
          num = va_arg(ap, unsigned long);
        else
          num = va_arg(ap, unsigned int);
        goto number;

      number:
        if (sign && (intmax_t)num < 0) {
          neg = 1;
          num = -(intmax_t)num;
        }
        if (neg)
          PCHAR('-');
        if (base == 16) {
          PCHAR('0');
          PCHAR('x');
        }
        p = print_num(nbuf, num, base, &n);
        while (n--)
          PCHAR(*p--);
        break;

      default:
        while (percent < fmt)
          PCHAR(*percent++);
        stop = 1;
        break;
    }
  }
#undef PCHAR

print:
  return write(fd, line, linelen);
}

void safe_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  safe_vprintf(STDERR_FILENO, fmt, ap);
  va_end(ap);
}

void safe_error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  safe_vprintf(STDERR_FILENO, fmt, ap);
  va_end(ap);
  exit(EXIT_FAILURE);
}
