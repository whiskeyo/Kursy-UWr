var Foo = (function() {
    function Bar() {
        Qux();
        console.log("Qux called from Bar method!");
    }

    function Qux() {
        console.log("Qux called (just called itself).");
    }

    function Foo() {}

    // Tutaj definiujemy które funkcje są widoczne z zewnątrz,
    // jeżeli jakiejś nie zapiszemy, to będzie prywatna, dzięki
    // czemu Object.getPrototypeOf(foo) zwróci tylko metody
    // zdefiniowane przez Foo.prototype.[nazwa] = [nazwa].
    Foo.prototype.Bar = Bar;    // metoda publiczna
    // Foo.prototype.Qux = Qux; // metoda prywatna

    return Foo;
}());

console.log(Object.getPrototypeOf(Foo)); // to jest funkcja

var foo = new Foo(); // przypisanie funkcji konstruktorowej do foo
console.log(Object.getPrototypeOf(foo));
console.log(Object.getPrototypeOf(foo.Bar));    // zwraca [Function]
// console.log(Object.getPrototypeOf(foo.Qux)); // to zwróciłoby błąd
foo.Bar();