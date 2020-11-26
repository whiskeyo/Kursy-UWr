#include "csapp.h"
#include "signal.h"

static pid_t spawn(void (*fn)(void))
{
	pid_t pid = Fork();
	if (pid == 0)
	{
		fn();
		printf("(%d) I'm done!\n", getpid());
		exit(EXIT_SUCCESS);
	}
	return pid;
}

static void grandchild(void)
{
	printf("(%d) Waiting for signal!\n", getpid());
	// signal(SIGINT, handler); // tego mozna sie pozbyc
	pause();
	printf("(%d) Got the signal!\n", getpid());
}

static void child(void)
{
	Setpgid(getpid(), getpid());
	pid_t pid = spawn(grandchild);
	printf("(%d) Grandchild (%d) spawned!\n", getpid(), pid);
}

/* Runs command "ps -o pid,ppid,pgrp,stat,cmd" using execve(2). */
static void ps(void)
{
	char* const argv[] = { "/usr/bin/ps", "-o", "pid,ppid,pgrp,stat,cmd", '\0'};
	execve(argv[0], argv, NULL);
}

int main(void)
{
	// Tworzymy zniwiarza:
#ifdef LINUX
	Prctl(PR_SET_CHILD_SUBREAPER, 1);
#endif
	printf("(%d) I'm a reaper now!\n", getpid());

	pid_t pid, pgrp;
	int status;

	// Spawnujemy dziecko, ktore spawnuje wnuka
	pgrp = spawn(child);
	Waitpid(pgrp, NULL, 0);

	// Odpalamy ps zeby sprawdzic liste procesow
	pid = spawn(ps);
	Waitpid(pid, NULL, 0);

	// Zabijamy proces dziecka
	Kill(-pgrp, SIGINT);

	printf("(%d) SIGINT sent to group %d!\n", getpid(), pgrp);
	Waitpid(-1, &status, 0);
	printf("(%d) Grandchild exit code is %d!\n", getpid(), WEXITSTATUS(status));

	return EXIT_SUCCESS;
}
