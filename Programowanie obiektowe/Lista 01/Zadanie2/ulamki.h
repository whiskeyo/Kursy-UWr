typedef struct u
{
    int licz;
    int mian;
} Ulamki;

int NWD(int a, int b);
int NWW(int a, int b);

void skroc(Ulamki *u);
void wyswietl(Ulamki *u);

// Funkcje modyfikujace drugi argument
void dodaj_mod(Ulamki *u1, Ulamki *u2);
void odejmij_mod(Ulamki *u1, Ulamki *u2);
void pomnoz_mod(Ulamki *u1, Ulamki *u2);
void podziel_mod(Ulamki *u1, Ulamki *u2);

// Funkcje zwracajace wskaznik
Ulamki *dodaj_wsk(Ulamki u1, Ulamki u2);
Ulamki *odejmij_wsk(Ulamki u1, Ulamki u2);
Ulamki *pomnoz_wsk(Ulamki u1, Ulamki u2);
Ulamki *podziel_wsk(Ulamki u1, Ulamki u2);
