let tab = [1, 2, 3]

const example = {
    x: 1,
    y: 1000
}

// Dla liczby i stringa przechowujacego sama liczbe
// wszystko jest ok, tzn. dostaniemy sie do elementu,
// do ktorego chcielismy sie dostac. Jak argumentem
// jest inny obiekt, to zwróci nam wartość undefined.
console.log(tab[2])
console.log(tab['2'])
console.log(tab[example]) // undefined
console.log(tab['example.x']) // undefined
console.log(tab[example.x]) // to zadziala

// Po dopisaniu wartości o kluczu niebędącego liczbą
// dopisany zostanie kolejny element na koniec tablicy 
tab[10] = 5
tab['xd'] = 10
tab['dx'] = 15
console.log(tab[5]) // tu nie ma zadnego elementu
console.log(tab['xd'])
console.log(tab[10])
console.log(tab)

// Uzycie [] sprawia, ze wyrazenie jest ewaluowane,
// a nastepnie uzywane, tzn. dla x[prop] oraz
// prop = a otrzymamy x.a, dla prop = b bedzie x.b
// Uzycie . sprawia, ze wyrazenie jest traktowane
// doslownie, nie jako wyrazenie do ewaluacji
console.log(tab.length)
console.log(tab['length'])

// Zmiana wartości właściwości length działa następująco:
// zmniejszenie tej wartości ogranicza liczbę elementów
// wyświetlanych przez console.log(tab), zwiększenie
// tej wartości dodaje puste miejsca w tablicy. Elementy
// o kluczach innych niz typu number nie wliczaja sie do
// tego ograniczenia i sa wyswietlane zawsze.
tab.length = 2
console.log(tab)

tab.length = 20
console.log(tab)