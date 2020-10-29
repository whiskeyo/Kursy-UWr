#include <iostream>
#include <limits>
#include <cmath>

int main()
{
    using lli = long long int;

    constexpr auto min = std::numeric_limits<lli>::min();
    constexpr auto max = std::numeric_limits<lli>::max();
    constexpr auto sizeofLli = sizeof(lli) * 8;
    auto decDigits = std::floor(std::log10(max)) + 1;

    std::cout << "Minimal value: " << min << std::endl;
    std::cout << "Maximal value: " << max << std::endl;
    std::cout << "Size in bits: " << sizeofLli << std::endl;
    std::cout << "Number of decimal digits: " << decDigits << std::endl;

    return 0;
}