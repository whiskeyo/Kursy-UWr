#include <stdio.h>
#include "ulamki.h"

int NWD(int a, int b)
{
    int reszta = a % b;
    if (reszta == 0)
        return b;

    return NWD(b, reszta);
}

int NWW(int a, int b)
{
    return a / NWD(a, b) * b;
}

void skroc(Ulamki *u)
{
    int nwd = NWD(u->licz, u->mian);
    u->licz /= nwd;
    u->mian /= nwd;

    if (u->mian < 0)
    {
        u->licz *= -1;
        u->mian *= -1;
    }
}

void wyswietl(Ulamki *u)
{
    printf("\nWartosc ulamka = %d/%d", u->licz, u->mian);
}

// Funkcje modyfikujace drugi argument
void dodaj_mod(Ulamki *u1, Ulamki *u2)
{
    Ulamki temp = *u1;

    temp.licz = u1->licz * u2->mian + u2->licz * u1->mian;
    temp.mian = u1->mian * u2->mian;

    u2->licz = temp.licz;
    u2->mian = temp.mian;

    skroc(u2);
}

void odejmij_mod(Ulamki *u1, Ulamki *u2)
{
    u2->licz *= -1;
    dodaj_mod(u1, u2);
}

void pomnoz_mod(Ulamki *u1, Ulamki *u2)
{
    u2->licz *= u1->licz;
    u2->mian *= u1->mian;
    skroc(u2);
}

void podziel_mod(Ulamki *u1, Ulamki *u2)
{
    int temp = u2->licz;
    u2->licz = u2->mian;
    u2->mian = temp;
    pomnoz_mod(u1, u2);
}

// Funkcje zwracajace wskaznik
Ulamki *dodaj_wsk(Ulamki u1, Ulamki u2)
{
    Ulamki *odp = malloc(sizeof(Ulamki));

    odp->licz = u1.licz * u2.mian + u2.licz * u1.mian;
    odp->mian = u1.mian * u2.mian;
    skroc(odp);

    return odp;
}

Ulamki *odejmij_wsk(Ulamki u1, Ulamki u2)
{
    u2.licz *= -1;
    return dodaj_wsk(u1, u2);
}

Ulamki *pomnoz_wsk(Ulamki u1, Ulamki u2)
{
    Ulamki *odp = malloc(sizeof(Ulamki));

    odp->licz = u1.licz * u2.licz;
    odp->mian = u1.mian * u2.mian;
    skroc(odp);

    return odp;
}

Ulamki *podziel_wsk(Ulamki u1, Ulamki u2)
{
    Ulamki *odp = malloc(sizeof(Ulamki));

    odp->licz = u1.licz * u2.mian;
    odp->mian = u1.mian * u2.licz;
    skroc(odp);

    return odp;
}
