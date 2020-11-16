module.exports = { func_b };
let module1 = require('./zadanie3a');

function func_b(n) {
    if (n > 0) {
        console.log(`func_b: ${n}`);
        module1.func_a(n - 1);
    }
}