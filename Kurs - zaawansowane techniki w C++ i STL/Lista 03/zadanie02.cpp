#include <iostream>
#include <limits>

int main()
{
    constexpr auto denormMinFloat  = std::numeric_limits<float>::denorm_min();
    constexpr auto denormMinDouble = std::numeric_limits<double>::denorm_min();
    constexpr auto maxFloat        = std::numeric_limits<float>::max();
    constexpr auto maxDouble       = std::numeric_limits<double>::max();
    constexpr auto epsilonFloat    = std::numeric_limits<float>::epsilon();
    constexpr auto epsilonDouble   = std::numeric_limits<double>::epsilon();

    std::cout << "Smallest float:\t" << denormMinFloat << std::endl;
    std::cout << "Smallest double:" << denormMinDouble << std::endl;
    std::cout << "Max float:\t" << maxFloat << std::endl;    
    std::cout << "Max double:\t" << maxDouble << std::endl;
    std::cout << "Epsilon float:\t" << epsilonFloat << std::endl;
    std::cout << "Epsilon double:\t" << epsilonDouble << std::endl;

    return 0;
}