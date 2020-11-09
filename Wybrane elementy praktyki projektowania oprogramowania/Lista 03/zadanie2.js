function fib(n) {
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return 1;

    return fib(n - 1) + fib(n - 2)
}

mem_fib = (function memoize() {
    // Cache ze wszystkimi obliczonymi wartosciami:
    var cache = {};

    // Funkcja dziala jak zwykly fib, jedynie dodane
    // jest sprawdzanie i dopisywanie wartocsi do cache
    function fib_memo(n) {
        var value;

        if (n in cache) {
            value = cache[n]
        } else {
            if (n == 0)
                value = 0;
            else if (n == 1 || n == 2)
                value = 1;
            else
                value = fib_memo(n - 1) + fib_memo(n - 2)
        }

        cache[n] = value;
        return value;
    }

    return fib_memo;
})();

// Sprawdzenie czasu dzialania funkcji:
console.time('mem_fib')
console.log(mem_fib(50))
console.timeEnd('mem_fib')

console.time('mem_fib (with cache saved)')
console.log(mem_fib(50))
console.timeEnd('mem_fib (with cache saved)')

console.time('fib')
console.log(fib(46))
console.timeEnd('fib')

// mem_fib: 3.801ms
// mem_fib (with cache saved): 0.071ms
// fib: 11071.160ms