function forEach(a, f) {
    for (var i = 0; i < a.length; i++)
        f(a[i])
}

function map(a, f) {
    for (var i = 0; i < a.length; i++)
        a[i] = f(a[i])
}

function filter(a, f) {
    for (var i = 0; i < a.length; i++)
        if (!f(a[i])) {
            a.splice(i, 1); // i-ty wyraz do usuniecia
            i--; // zmniejszamy indeks, zeby przejsc przez
            // wszystkie elementy tablicy a
        }

}

var a = [1, 2, 3, 4];

forEach(a, _ => { console.log(_); }); // otrzymujemy [1,2,3,4]


filter(a, _ => _ < 3);
console.log(a) // otrzymujemy [1,2]

a = [1, 2, 3, 4];
map(a, _ => _ * 2);
console.log(a) // otrzymujemy [2,4,6,8]