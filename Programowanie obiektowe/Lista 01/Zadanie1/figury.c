#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "figury.h"

float dlugosc_kw_odc(float x1, float y1, float x2, float y2)
{
    return ( pow(x2 - x1, 2) + pow(y2 - y1, 2) );
}

float dlugosc_odc(float x1, float y1, float x2, float y2)
{
    return ( sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)) );
}

int jestKwadratem(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    float a = dlugosc_kw_odc(x1, y1, x2, y2); // kwadrat odleglosci od A do B
    float b = dlugosc_kw_odc(x1, y1, x3, y3); // kwadrat odleglosci od A do C
    float c = dlugosc_kw_odc(x1, y1, x4, y4); // kwadrat odleglosci od A do D

    // Gdy dlugosci AB oraz AC sa takie same, ponizsze warunki musza byc spelnione, aby figura byla kwadratem
    // 1) Kwadrat dlugosci AD jest rowny podwojonemu kwadratowi AB
    // 2) Kwadrat dlugosci BC jest rowny podwojonemu kwadratowi AB

    if (a == b && 2 * a == c && 2 * a == dlugosc_kw_odc(x2, y2, x3, y3))
    {
        int d = dlugosc_kw_odc(x2, y2, x4, y4);
        return (d == dlugosc_kw_odc(x3, y3, x4, y4) && d == a);
    }

    if (b == c && 2 * b == a && 2 * b == dlugosc_kw_odc(x3, y3, x4, y4))
    {
        int d = dlugosc_kw_odc(x2, y2, x3, y3);
        return (d == dlugosc_kw_odc(x2, y2, x4, y4) && d == b);
    }

    if (a == c && 2 * a == b && 2 * a == dlugosc_kw_odc(x2, y2, x4, y4))
    {
        int d = dlugosc_kw_odc(x2, y2, x3, y3);
        return (d == dlugosc_kw_odc(x3, y3, x4, y4) && d == a);
    }

    return 0;
}

float pole(Figura *f)
{
    float p;

    if (f->typ == KOLO)
        p = M_PI * f->r * f->r;

    else if (f->typ == KWADRAT) // zakladamy ze boki sa rownolegle do osi wspolrzednych
    {
        float dlugosc_boku = dlugosc_odc(f->x1, f->y1, f->x2, f->y2);
        p = pow(dlugosc_boku, 2);
    }

    else // dla trojkata wykorzystuje wzor Herona
    {
        float a = dlugosc_odc(f->x1, f->y1, f->x2, f->y2);
        float b = dlugosc_odc(f->x2, f->y2, f->x3, f->y3);
        float c = dlugosc_odc(f->x1, f->y1, f->x3, f->y3);

        float x = 0.5 * (a + b + c); // x oznacza polowe obwodu
        p = sqrt(x * (x - a) * (x - b) * (x - c));
    }

    return p;
}

void przesun(Figura *f, float x, float y)
{
    f->x1 += x;
    f->y1 += y;

    if (f->typ != KOLO)
    {
        f->x2 += x;
        f->y2 += y;
        f->x3 += x;
        f->y3 += y;
    }

    if (f->typ == KWADRAT)
    {
        f->x4 += x;
        f->y4 += y;
    }
}

float sumapol(Figura *f, int size)
{
    float suma = 0;

    for (int i = 0; i < size; i++)
    {
        suma += pole(f++);
    }

    return suma;
}

void wczytaj_dane(Figura *f)
{
    puts("Podaj typ figury wprowadzajac odpowiednia cyfre (0 - trojkat, 1 - kwadrat, 2 - kolo):");
    if (scanf("%u", &f->typ) != 1 || (int)f->typ > 2 || (int)f->typ < 0)
    {
        puts("Wprowadzono niepoprawny typ figury.");
        exit(-1);
    }

    puts("Podaj wspolrzedne kolejnych wierzcholkow (x1, y1, x2, y2,...) lub x, y oraz r dla kola:");

    if (f->typ == TROJKAT)
    {
        if (scanf("%f%f%f%f%f%f", &f->x1, &f->y1, &f->x2, &f->y2, &f->x3, &f->y3) != 6)
        {
            puts("Niepoprawna liczba argumentow.");
            exit(-1);
        }

        // Obliczenie dlugosci bokow:
        float a = dlugosc_odc(f->x1, f->y1, f->x2, f->y2);
        float b = dlugosc_odc(f->x2, f->y2, f->x3, f->y3);
        float c = dlugosc_odc(f->x1, f->y1, f->x3, f->y3);

        // Sprawdzenie czy trojkat o takich bokach istnieje:
        if (a + b <= c || a + c <= b || b + c <= a)
        {
            puts("Punkty o takich wspolrzednych nie tworza trojkata.");
            exit(-1);
        }
    }

    else if (f->typ == KWADRAT)
    {
        if (scanf("%f%f%f%f%f%f%f%f", &f->x1, &f->y1, &f->x2, &f->y2, &f->x3, &f->y3, &f->x4, &f->y4) != 8)
        {
            puts("Niepoprawna liczba argumentow.");
            exit(-1);
        }

        // Sprawdzenie czy kwadrat o takich bokach istnieje:
        if (!(jestKwadratem(f->x1, f->y1, f->x2, f->y2, f->x3, f->y3, f->x4, f->y4)))
        {
            puts("Punkty o takich wspolrzednych nie tworza kwadratu.");
            exit(-1);
        }
    }

    else // kolo
    {
        if (scanf("%f%f%f", &f->x1, &f->y1, &f->r) != 3 || f->r < 0)
        {
            puts("Nieprawidlowa liczba argumentow lub promien mniejszy od 0.");
            exit(-1);
        }
    }
}

void wyswietl_info(Figura *f)
{
    switch(f->typ)
    {
        case TROJKAT:
            puts("Wybrana figura jest trojkatem.");
            printf("Wierzcholki: (%f, %f), (%f, %f), (%f, %f)\n",
                    f->x1, f->y1, f->x2, f->y2, f->x3, f->y3);
            break;

        case KWADRAT:
            puts("Wybrana figura jest kwadratem.");
            printf("Wierzcholki: (%f, %f), (%f, %f), (%f, %f), (%f, %f)\n",
                    f->x1, f->y1, f->x2, f->y2, f->x3, f->y3, f->x4, f->y4);
            break;

        case KOLO:
            puts("Wybrana figura jest kolem.");
            printf("Srodek kola: (%f, %f), promien = %f\n", f->x1, f->y1, f->r);
            break;
    }
    printf("Pole = %f", pole(f));
}
