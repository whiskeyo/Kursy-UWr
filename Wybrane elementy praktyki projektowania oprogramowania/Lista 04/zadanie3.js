// Moduły są nazwane zadanie3a.js oraz zadanie3b.js.
// Można je zaimplementować w taki sposób, aby tworzyły
// cykl. W pliku głównym (zadanie3.js) używamy modułu A,
// w którym wywołujemy funkcję func_a. W module A jednak
// używany jest moduł B, w którym znajduje się funkcja
// func_b. Funkcje z obu modułów wywołują się naprzemiennie,
// co pokazuje że można zaimplementować cykl w modułach.

let x = require('./zadanie3a');
x.func_a(10);