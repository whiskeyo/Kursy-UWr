const fs   = require('fs');
const util = require('util');

// Normalna funkcja wykorzystująca fs::readFile
fs.readFile('textfile.txt', 'utf-8', function(err, data) {
    console.log("textfile.txt + ordinary function:\n" + data);
});

// Funkcja przyjmująca argumenty fs::readFile, zwraca Promise
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

// Wykorzystanie util.promisify z biblioteki standardowej
const readFileAsync = util.promisify(fs.readFile);

readFileAsync('textfile.txt', 'utf-8').then(data => {
    console.log("textfile.txt + util.promisify:\n", data);
});

// Wykorzystanie fs.promises z biblioteki standardowej
fs.promises.readFile('textfile.txt', 'utf-8').then(data => {
    console.log("textfile.txt + fs.promises:\n", data);
});


// Wywołanie "po staremu", a więc z kontynuacją Promise::then
readPromise('textfile.txt', 'utf-8').then(data => {
    console.log("textfile.txt + then:\n", data);
});

// Wywołanie "po nowemu", a więc przez async/await
async function readAsync() {
    var data = await readPromise('textfile.txt', 'utf-8');
    console.log("textfile.txt + await:\n", data);
};

readAsync();