module.exports = { func_a };
let module2 = require('./zadanie3b');

function func_a(n) {
    if (n > 0) {
        console.log(`func_a: ${n}`);
        module2.func_b(n - 1);
    }
}