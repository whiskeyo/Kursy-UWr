#include "csapp.h"
#include "terminal.h"

#undef MAXLINE
#define MAXLINE 120

static sigjmp_buf env;

static void signal_handler(int signo) {
  /* TODO: Something is missing here! */
  siglongjmp(env, signo);
}

/* If interrupted by signal, returns signal number. Otherwise converts user
 * provided string to number and saves it under num_p and returns zero. */
static int readnum(int *num_p) {
  char line[MAXLINE];
  int n;

  /* TODO: Something is missing here! Use Read() to get line from user. */
  alarm(1);
  n = sigsetjmp(env, 0);

  if (n) {
    *num_p = 0;
    return n;
  }

  Read(STDIN_FILENO, line, MAXLINE);
  *num_p = atoi(line);
  return 0;
}

static void game(void) {
  int tty = tty_open();

  int timeout = 0, num1 = 0, num2 = 0, sum;
  int last_sig = 0;
  int lives = 3;
  int score = 0;

  while (lives > 0) {
    switch (last_sig) {
      case 0:
        timeout = 5;
        num1 = random() % 100;
        num2 = random() % 100;
        printf("What is the sum of %d and %d?\n", num1, num2);
        break;

      case SIGINT:
        printf(CHA(1) EL() "Bye bye!\n");
        exit(EXIT_FAILURE);

      case SIGALRM:
        timeout--;
        if (timeout < 0) {
          last_sig = 0;
          lives--;
          printf(CHA(1) EL() "Answer was %d!\n", num1 + num2);
          continue;
        }
        break;

      default:
        app_error("lastsig = %d not handled!\n", last_sig);
        break;
    }

    /* Rewrite user prompt to show current number of lives and timeout. */
    sigset_t set, oldset;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGALRM);
    Sigprocmask(SIG_BLOCK, &set, &oldset);

    int x, y;
    tty_curpos(tty, &x, &y);
    dprintf(STDOUT_FILENO, CHA(1) "lives: %d timeout: %d > ", lives, timeout);
    if (last_sig == SIGALRM)
      dprintf(STDOUT_FILENO, CHA(%d), y);

    Sigprocmask(SIG_SETMASK, &oldset, NULL);

    /* Read a number from user. */
    last_sig = readnum(&sum);
    if (last_sig)
      continue;

    /* Line contains user input (a number) terminated with '\0'. */
    if (sum == num1 + num2) {
      printf("Correct!\n");
      score++;
    } else {
      printf("Incorrect!\n");
      lives--;
    }
  }

  Close(tty);

  printf("Game over! Your score is %d.\n", score);
}

int main(void) {
  /* Initialize PRNG seed. */
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srandom(tv.tv_usec);

  /* SIGALRM is used for timeouts, SIGINT for graceful exit. */
  Signal(SIGALRM, signal_handler);
  Signal(SIGINT, signal_handler);

  game();

  return EXIT_SUCCESS;
}
