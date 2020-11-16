const readline = require('readline');
const fs       = require('fs');

var results = {}; // słownik [ip:wystąpienia]

// Stworzenie interfejsu do odczytania logów
// z pliku logs.txt linia po linii.
var myInterface = readline.createInterface({
    input: fs.createReadStream('logs.txt')
});

// Logi serwera WWW są stringiem postaci:
// 08:55:11 192.168.0.1 GET /TheApplication/WebResource.axd 200
// więc dzielimy całą linię separatorem ' ', a następnie 
// wybieramy drugi wyraz z tablicy, którym jest IP - jeśli 
// istnieje w słowniku, to zwiększamy licznik, a jeżeli pojawia 
// się pierwszy raz, to ustawiamy jego licznik na 1.
myInterface.on('line', function (line) {
    var ip = line.split(" ")[1];
    if (results.hasOwnProperty(ip))
        results[ip]++;
    else
        results[ip] = 1;
});

// Po zakończeniu odczytu pliku należy wszystkie dane przefiltrować,
// gdyż chcemy zwrócić TYLKO pierwsze trzy adresy IP z największą
// liczbą żądań do serwera.
myInterface.on('close', function() {
    var final_results = {
        1: ["ip1", 0], 
        2: ["ip2", 0], 
        3: ["ip3", 0]
    };

    // Przeglądamy wszystkie adresy żądań, a następnie sprawdzamy czy
    // któreś IP spoza listy TOP3 (najwięcej żądań) mają więcej wystąpień,
    // tzn. czy kwalifikują się do TOP3. Jeżeli tak, to ten wyraz dodajemy
    // do wyników, a ostatni z TOP3 kasujemy.
    for (var key in results) {
        if (results[key] > final_results[3][1]){
            var tuple = [key, results[key]];
            for (var x in final_results) {
                if (tuple[1] > final_results[x][1]) {
                    var temp = final_results[x];
                    final_results[x] = tuple;
                    tuple = temp;
                }
            }
        }
    }

    // Na koniec wyświetlamy wyniki:
    console.log(final_results[1][0], final_results[1][1]);
    console.log(final_results[2][0], final_results[2][1]);
    console.log(final_results[3][0], final_results[3][1]);
});