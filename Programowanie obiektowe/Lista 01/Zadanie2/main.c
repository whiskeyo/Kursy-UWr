#include <stdio.h>
#include "ulamki.h"

int main()
{
    Ulamki *u1 = malloc(sizeof(Ulamki));
    Ulamki *u2 = malloc(sizeof(Ulamki));
    Ulamki *odp;

    puts("Podaj dwa ulamki w postaci \'a/b c/d\'");
    scanf("%d/%d %d/%d", &u1->licz, &u1->mian, &u2->licz, &u2->mian);

    if (u1->mian == 0 || u2->mian == 0)
    {
        puts("Nie mozna dzielic przez zero.");
        exit(-1);
    }

    puts("Dzialania ze zwroceniem wskaznika (bez modyfikacji):");

    odp = dodaj_wsk(*u1, *u2);
    printf("\nDodawanie: %d/%d", odp->licz, odp->mian);

    odp = odejmij_wsk(*u1, *u2);
    printf("\nOdejmowanie: %d/%d", odp->licz, odp->mian);

    odp = pomnoz_wsk(*u1, *u2);
    printf("\nMnozenie: %d/%d", odp->licz, odp->mian);

    odp = podziel_wsk(*u1, *u2);
    printf("\nDzielenie: %d/%d", odp->licz, odp->mian);

    free(odp);

    puts("\n\nDzialania z modyfikacja drugiego argumentu:");
    dodaj_mod(u1, u2);
    wyswietl(u2);
    odejmij_mod(u1, u2);
    wyswietl(u2);
    pomnoz_mod(u1, u2);
    wyswietl(u2);
    podziel_mod(u1, u2);
    wyswietl(u2);

    free(u1);
    free(u2);
}
