function fibIter(n) {
    var a = 1,
        b = 0,
        temp;
    while (n >= 0) {
        temp = a;
        a = a + b;
        b = temp;
        n--;
    }

    return b;
}

function fibRec(n) {
    if (n <= 1) return 1;
    return fibRec(n - 1) + fibRec(n - 2);
}

console.time("1000th Fibonacci number iteratively");
fibIter(1000);
console.timeEnd("1000th Fibonacci number iteratively");

console.time("45th Fibonacci number recursively");
fibRec(45);
console.timeEnd("45th Fibonacci number recursively");