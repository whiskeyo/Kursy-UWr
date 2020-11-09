function fibGenerator(n) {
    return function fib() {
        var _f0 = 0;
        var _f1 = 1;
        var _temp = 0;

        return {
            next: function() {
                _temp = _f0;
                _f0 = _f1;
                _f1 = _temp + _f1;
                return {
                    value: _f1,
                    done: _f1 >= n
                };
            }
        };
    };
}

// Generator liczb Fibonacciego bez argumentu (wiec
// generuje az do nieskonczonosci i wysypania sie)
function* fibGeneratorYield(current = 0, next = 1) {
    yield current;
    yield* fibGeneratorYield(next, current + next);
}


// Generator n-liczb Fibonacciego z argumentem
function* fibGeneratorYieldN(n, current = 0, next = 1) {
    if (n === 0)
        return current;

    yield current;
    yield* fibGeneratorYieldN(n - 1, next, current + next);
}

// Generator iteratora dla liczb Fibonacciego dla n
// generuje kolejne liczby z przedzialu [1, n-1].
var n = 50;
var fibGen = fibGenerator(n);
var _it = fibGen();

for (var _result; _result = _it.next(), !_result.done;) {
    console.log(_result.value);
}

// Generator używający yield generuje liczby tak jak
// ten zwykly, bez argumentu:
var _itYield = fibGeneratorYield();
for (var _result;
    (_result = _itYield.next()), !_result.done, _result.value <= n;)
    console.log(_result.value);

// W przypadku generatora używającego yield z argumentem 
// generowane są wartości dla wszystkich liczb do F_n:
var _itYieldN = fibGeneratorYieldN(n);
for (var _result; _result = _itYieldN.next(), !_result.done;) {
    console.log(_result.value);
}

// Iterowanie za pomocą for-of nie działa dla zwyklego generatora:
// for (var i of fibGenerator())
//     console.log(i)
// TypeError: fibGenerator is not a function or its return value is not iterable

// Iterowanie za pomocą for-of po fibGeneratorYield działa:
for (var i of fibGeneratorYield()) {
    if (i <= n)
        console.log(i);
    else
        return;
}

// Iterowanie za pomocą for-of po fibGeneratorYieldN działa:
for (var i of fibGeneratorYieldN(n))
    console.log(i)