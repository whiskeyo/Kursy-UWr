function sum(...args) {
    var sum = 0;
    for (var i = 0; i < args.length; i++)
        sum += args[i]

    return sum;
}

console.log(sum())
console.log(sum(1, 2, 3))
console.log(sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))