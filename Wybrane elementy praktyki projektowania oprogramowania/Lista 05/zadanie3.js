const http = require('http');

var filename = 'file.txt';

http.createServer((req, res) => {
    res.setHeader('Content-Disposition', 'attachment; filename=' + filename);
    res.write('Content of downloaded file');
    res.end();
}).listen(3000);

console.log('Server started!');