#include <iostream>

// W tym zadaniu należy zaimplemenotwać funktor do
// składania funkcji, jednak mam wrażenie że lambda
// sama w sobie będzie dużo krótszym rozwiązaniem
// spełniającym wszystkie założenia z zadania.

int square(int& x) 
{ 
    x = x * x;
    std::cout << "Square function called! Result = " << x << std::endl;
    return x;
}

int increment(int& x)
{ 
    x = x + 1;
    std::cout << "Increment function called! Result = " << x << std::endl;
    return x;
}

int decrement(int& x)
{ 
    x = x - 1;
    std::cout << "Decrement function called! Result = " << x << std::endl;
    return x;
}

int main() 
{
    // To "złożenie funkcji" nie działa jako f(g(x)),
    // lecz wykonuje każdą funkcję po kolei, a więc
    // najpierw f(x), a później g(x). Nie interesują
    // nas zwracane wyniki, lecz samo wywołanie funkcji
    // w odpowiedniej kolejności. 
    auto call_in_order = [](auto f, auto g) {
        return [=](auto x) {
            f(x);
            g(x);
        };
    };

    // simple composition of two functions
    std::cout << "Example 1:" << std::endl;
    call_in_order(square, increment)(1); // square -> inc

    std::cout << "Example 2:" << std::endl;
    call_in_order(increment, square)(2); // inc -> square

    // composition of compositions
    std::cout << "Example 3:" << std::endl;
    call_in_order(call_in_order(square, increment), decrement)(3); // square -> inc -> dec

    std::cout << "Example 4:" << std::endl;
    call_in_order(decrement, call_in_order(square, increment))(4); // dec -> square -> inc
}