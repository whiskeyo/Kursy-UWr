function fibGenerator(n) {
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
                done: false
            };
        }
    };
}

function* fibGeneratorYield(current = 0, next = 1) {
    yield current;
    yield* fibGeneratorYield(next, current + next);
}

function* take(it, top) {
    let index = 0;
    while (index <= top) {
        yield it.next().value;
        ++index;
    }
}

var n = 10;

for (let num of take(fibGenerator(), n))
    console.log(num);

for (let num of take(fibGeneratorYield(), n))
    console.log(num);