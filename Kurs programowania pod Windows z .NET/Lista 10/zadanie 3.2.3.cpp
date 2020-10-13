#include <Windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>

// W ustawieniach projektu nalezy zmienic wlasiwosci linkera, tzn. w System -> Subsystem
// zmienic parametr Windows (/SUBSYSTEM:WINDOWS) na Console (/SUBSYSTEM:CONSOLE).

void dostawcaThread() 
{
	HANDLE sDostawca   = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_DOSTAWCA");
	HANDLE sTyton   = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_TYTON");
	HANDLE sBletki  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_BLETKI");
	HANDLE sZapalki = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_ZAPALKI");
	HANDLE sGlowny  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_GLOWNY");

	// Dostawca moze wystawic jednoczesnie dwa skladniki, wiec losuje dwa z trzech
	// i w semaforze przekazujemy parametr brakujacego skladnika, aby palacz, ktory
	// posiada brakujacy skladnik mogl bezproblemowo zapalic.
	while (true) 
	{
		WaitForSingleObject(sGlowny, INFINITE);
		int i = rand() % 3;
		switch (i) 
		{
			case 0:
				printf("Dostawca wystawia bletki i zapalki\n");
				ReleaseSemaphore(sTyton, 1, NULL);
				break;
			case 1:
				printf("Dostawca wystawia tyton i zapalki\n");
				ReleaseSemaphore(sBletki, 1, NULL);
				break;
			case 2:
				printf("Dostawca wystawia tyton i bletki\n");
				ReleaseSemaphore(sZapalki, 1, NULL);
				break;
			}
			ReleaseSemaphore(sGlowny, 1, NULL);
			WaitForSingleObject(sDostawca, INFINITE);
	}
}

void palaczThread(LPVOID* arg) 
{
	HANDLE sDostawca = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_DOSTAWCA");
	HANDLE sTyton    = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_TYTON");
	HANDLE sBletki   = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_BLETKI");
	HANDLE sZapalki  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_ZAPALKI");
	HANDLE sGlowny   = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "SEM_GLOWNY");

	// Palacze czekaja na wystawienie odpowiedniego skladnika, zeby mogli sobie skrecic
	// papierosa i zapalic, a nastepnie dostawca bedzie mogl dostawic kolejny skladnik.
	int i = (int)arg;
	for (int count = 0; count < 50; ++count) 
	{
		switch (i) 
		{
			case 0:
				WaitForSingleObject(sTyton, INFINITE);
				WaitForSingleObject(sGlowny, INFINITE);
				printf("Palacz z tytoniem zapala papierosa\n");
				ReleaseSemaphore(sDostawca, 1, NULL);
				ReleaseSemaphore(sGlowny, 1, NULL);
				break;
			case 1:
				WaitForSingleObject(sBletki, INFINITE);
				WaitForSingleObject(sGlowny, INFINITE);
				printf("Palacz z blotkami zapala papierosa\n");
				ReleaseSemaphore(sDostawca, 1, NULL);
				ReleaseSemaphore(sGlowny, 1, NULL);
				break;
			case 2:
				WaitForSingleObject(sZapalki, INFINITE);
				WaitForSingleObject(sGlowny, INFINITE);
				printf("Palacz z zapalkami zapala papierosa\n");
				ReleaseSemaphore(sDostawca, 1, NULL);
				ReleaseSemaphore(sGlowny, 1, NULL);
				break;
		}
	}
}


int main() 
{
	srand((unsigned)time(NULL));
	HANDLE dostawca, palacze[3];
	HANDLE sDostawca = CreateSemaphore(NULL, 0, 1, "SEM_DOSTAWCA");
	HANDLE sTyton    = CreateSemaphore(NULL, 0, 1, "SEM_TYTON");
	HANDLE sBletki   = CreateSemaphore(NULL, 0, 1, "SEM_BLETKI");
	HANDLE sZapalki  = CreateSemaphore(NULL, 0, 1, "SEM_ZAPALKI");
	HANDLE sGlowny   = CreateSemaphore(NULL, 1, 1, "SEM_GLOWNY");


	for (int i = 0; i < 3; ++i) 
		palacze[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)palaczThread, (LPVOID)i, NULL, NULL);

	dostawca = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dostawcaThread, NULL, NULL, NULL);
	WaitForMultipleObjects(3, palacze, true, INFINITE);
	for (int i = 0; i < 3; ++i) 
	{
		TerminateThread(palacze[i], 0);
		CloseHandle(palacze[i]);
	}

	TerminateThread(dostawca, 0);
	CloseHandle(dostawca);
	getchar();
	return 0;
}