#include "csapp.h"
#include "signal.h"

static void signal_handler(int signum, siginfo_t *info, void *data)
{
    if (signum == SIGINT)
	{
		safe_printf("(%d) Screw you guys... I'm going home!\n", getpid());
		_exit(0);
	}
}

static void play(pid_t next, const sigset_t *set)
{
	for (;;)
	{
		printf("(%d) Waiting for a ball!\n", getpid());
		Sigsuspend(set); // tutaj sobie czekamy na sygnal (pilke), zawieszamy proces do otrzymania kolejnego sygnalu
		usleep((300 + random() % 400) * 1000);
		Kill(next, SIGUSR1);
		printf("(%d) Passing ball to (%d)!\n", getpid(), next);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		app_error("Usage: %s [CHILDREN]", argv[0]);

	int children = atoi(argv[1]);

	if (children < 4 || children > 20)
		app_error("Give number of children in range from 4 to 20!");

	/* Register signal handler for SIGUSR1 */
	struct sigaction action = {.sa_sigaction = signal_handler};
	Sigaction(SIGINT, &action, NULL);
	Sigaction(SIGUSR1, &action, NULL);
     
	sigemptyset(&action.sa_mask); // ustawienie maski na 0
    sigaddset(&action.sa_mask, SIGUSR1); // obsluga sygnalu SIGUSR1 (pilka)
    Sigprocmask(SIG_BLOCK, &action.sa_mask, NULL);

    pid_t next_pid = getpid();

    for (int i = 0; i < children; i++)
    {
        pid_t pid = Fork();

        if (!pid)
            play(next_pid, &action.sa_mask);
        else
            next_pid = pid;
    }

    Kill(next_pid, SIGUSR1);
    play(next_pid, &action.sa_mask);

	return EXIT_SUCCESS;
}
