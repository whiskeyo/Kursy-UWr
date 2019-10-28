#include <stdio.h>
#include <stdlib.h>
#include "figury.h"

int main()
{
    Figura *f = malloc(sizeof(Figura));
    wczytaj_dane(f);
    wyswietl_info(f);

    przesun(f, 6.12, -1.24);
    puts("\n\nPo przesunieciu:");
    wyswietl_info(f);

    Figura fig[4];
    fig[0] = fig[1] = fig[2] = fig[3] = *f;
    printf("\n\nSuma 4 pol tej figury = %f", sumapol(f, 4));

    free(f);
}
