#include <iostream>
#include <ratio>

template <int N>
struct harmonic : public std::ratio_add<std::ratio<1, N>, harmonic<N - 1>> {};

template <>
struct harmonic<1> : public std::ratio<1, 1> {};

int main() 
{
    constexpr int i = 46; // dla wyzszych wartosci jest overflow in addition
    std::cout << "H[" << i << "] = " << harmonic<i>::num << " / " << harmonic<i>::den << std::endl;

    return 0;
}