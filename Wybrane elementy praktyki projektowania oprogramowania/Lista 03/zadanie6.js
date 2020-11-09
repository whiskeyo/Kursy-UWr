// Iterator z zadania ze stala 10 wraz z obiektem
// umozliwiajacym iterowanie jego zawartosci:
function createGenerator() {
    var _state = 0;
    return {
        next: function() {
            return {
                value: _state,
                done: _state++ >= 10
            }
        }
    }
}

// var foo = {
//     [Symbol.iterator]: createGenerator
// };

// for (var f of foo)
//     console.log(f);

// Iterator sparametryzowany oraz obiekty korzystajace
// z generatora zainicjowanego roznymi wartosciami:
function createParamGenerator(elemsCount) {
    return function createGenerator() {
        var _state = 0;
        return {
            next: function() {
                return {
                    value: _state,
                    done: _state++ >= elemsCount
                };
            }
        };
    };
}

var foo1 = {
    [Symbol.iterator]: createParamGenerator(3)
};

var foo2 = {
    [Symbol.iterator]: createParamGenerator(13)
};

console.log("Elements of foo1:");
for (var item of foo1)
    console.log(item);

console.log("-----------------");
console.log("Elements of foo2:");
for (var item of foo2)
    console.log(item);