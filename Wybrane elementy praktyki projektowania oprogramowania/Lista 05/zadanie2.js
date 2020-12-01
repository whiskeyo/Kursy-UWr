// openssl genrsa -out key.pem
// openssl req -new -key key.pem -out csr.pem (haslo 1234)
// openssl x509 -req -days 9999 -in csr.pem -signkey key.pem -out cert.pem

// serwer można odpalić poleceniem:
// $ node zadanie2.js (linux)
// > node.exe .\zadanie2.js (win10)

const fs = require('fs');
const https = require('https');

const options = {
    key:  fs.readFileSync('key.pem'),
    cert: fs.readFileSync('cert.pem')
};

https.createServer(options, (req, res) => {
    res.setHeader('Content-type', 'text/html; charset=utf-8');
    res.end(`Hello, the time now is ${new Date()}.`);
}).listen(3000);

console.log('Server started!');