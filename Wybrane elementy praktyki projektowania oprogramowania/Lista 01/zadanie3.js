function isPrime(n) {
    for (var i = 2; i < n; i++)
        if (n % i === 0)
            return false;

    return true;
}

function getPrimes(n) {
    var primes = [2];
    for (var i = 3; i < n; i += 2) // dzieki temu nie sprawdzamy parzystych
        if (isPrime(i))
            primes.push(i);

    console.log(primes);
}

getPrimes(100000);