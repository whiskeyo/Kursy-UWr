const fs = require('fs');

// Takie podejście jest o tyle słabe, że nie zapisuje
// wszystkiego do pamięci, tylko wypisuje cały plik.
function read(path, encoding) {
    return fs.readFile(path, encoding, (err, data) => {
        if (err) {
            console.error(err);
            return;
        }
        console.log(data);
    })
};

read('textfile.txt', 'utf-8');

// Można to obejść używając obiektu Promise, dzięki
// czemu wszystkie dane zapiszemy w jego stanie.
function readPromise(path, encoding) {
    return new Promise((resolve, reject) => {
        fs.readFile(path, encoding, (err, data) => {
            if (err) {
                console.log(err);
                reject(err);
            }
            resolve(data);
        })
    })
};

readPromise('textfile.txt', 'utf-8').then(data => {
    console.log(data);
});

// Można to również zrobić asynchronicznie:
(async function() {
    var result = await fs.promises.readFile('textfile.txt', 'utf-8');
    console.log(result);
})();