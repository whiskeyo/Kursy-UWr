#include<iostream>

// W tym zadaniu należy zaimplemenotwać funktor do
// składania funkcji, jednak mam wrażenie że lambda
// sama w sobie będzie dużo krótszym rozwiązaniem
// spełniającym wszystkie założenia z zadania.

int square(int x)    { return x * x; }
int increment(int x) { return x + 1; }
int decrement(int x) { return x - 1; }

int main() 
{
    auto compose = [](auto f, auto g) {
        return [=](auto x) {
            return f(g(x));
        };
    };

    // simple composition of two functions
    auto comp1 = compose(square, increment); // (x + 1)^2
    auto comp2 = compose(increment, square); // x^2 + 1

    std::cout << comp1(10) << std::endl;
    std::cout << comp2(10) << std::endl;

    // composition of compositions
    auto comp3 = compose(compose(square, increment), decrement);
    auto comp4 = compose(decrement, compose(square, increment));

    std::cout << comp3(10) << std::endl;
    std::cout << comp4(10) << std::endl;
}