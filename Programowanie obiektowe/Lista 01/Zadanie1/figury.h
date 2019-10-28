
enum t {TROJKAT, KWADRAT, KOLO};
typedef enum t typfig;

typedef struct f
{
    typfig typ;
    float x1, x2, x3, x4, y1, y2, y3, y4;
    float r; // tylko dla kola
} Figura;

float pole(Figura *f);
void przesun(Figura *f, float x, float y);
float sumapol(Figura *f, int size);
void wczytaj_dane(Figura *f);
void wyswietl_info(Figura *f);

// Funkcje pomocnicze:

float dlugosc_kw_odc(float x1, float y1, float x2, float y2);
float dlugosc_odc(float x1, float y1, float x2, float y2);
int jestKwadratem(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
