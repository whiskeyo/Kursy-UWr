// Zastapienie var na let w ciele petli for sprawia,
// ze kazda iteracja petli bedzie tworzyla nowy zmienna
// i w zasiegu petli:
function createFs(n) { // tworzy tablicę n funkcji
    var fs = []; // i-ta funkcja z tablicy ma zwrócić i
    for (let i = 0; i < n; i++) {
        fs[i] = function() { return i; };
    };
    return fs;
}

// Mozna rowniez dodac kolejny poziom zagniezdzenia,
// wtedy w kazdej iteracji bedzie tworzony nowy 
// kontekst wiazania domkniecia.
function createFsNew(n) { // tworzy tablicę n funkcji
    var fs = []; // i-ta funkcja z tablicy ma zwrócić i
    for (var i = 0; i < n; i++) {
        fs[i] = (function() {
            var j; // bez tego bedziemy nadal pracowac
            j = i; // ze starym i (var), przez co nadal
            // nie bedzie dzialac jak powinno
            return function() { return j; };
        })();
    };
    return fs;
}
var myfs = createFs(10);
var myfsnew = createFsNew(10);

console.log("Funkcja createFs po zmianie var na let:");
console.log(myfs[0]()); // zerowa funkcja zwraca 0
console.log(myfs[2]()); // druga zwraca 2
console.log(myfs[7]()); // trzecia zwraca 7

console.log("Funkcja createFsNew z dodatkowym zagniezdzeniem:");
console.log(myfsnew[0]()); // zerowa funkcja zwraca 0
console.log(myfsnew[2]()); // druga zwraca 2
console.log(myfsnew[7]()); // trzecia zwraca 7