function getAllDigits(n) {
    var digits = [];

    while (n) {
        digits.push(n % 10);
        n = Math.floor(n / 10);
    }

    return digits;
}

function isGood(n) {
    var digits = getAllDigits(n);
    for (var i = 0; i < digits.length; i++) {
        if (digits[i] == 0 || n % digits[i] != 0)
            return false;
    }

    var sumOfDigits = digits.reduce((accumulator, element) => accumulator += element);
    return (n % sumOfDigits == 0);
}

var set = new Set();

for (var i = 1; i <= 100000; i++)
    if (isGood(i))
        set.add(i);

console.log(set);