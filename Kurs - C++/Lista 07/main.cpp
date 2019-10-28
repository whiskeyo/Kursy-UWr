#include <iostream>
#include <climits>
#include <cstdio>
#include "rational.hpp"

using namespace calculations;

int main()
{
	Rational r1(1, 5);
	Rational r2(7, 3);
	std::cout << "1/5 = " << r1 << std::endl;
    std::cout << "7/3 = " << r2 << std::endl;

    std::cout << "1/5 + 7/3 = " << r1 + r2 << std::endl;
    std::cout << "1/5 - 7/3 = " << r1 - r2 << std::endl;
    std::cout << "1/5 * 7/3 = " << r1 * r2 << std::endl;
    std::cout << "1/5 / 7/3 = " << r1 / r2 << std::endl;

    std::cout << "r1 (1/5) += 7/3 equals " << (r1 += r2) << std::endl;
    std::cout << "r1 (new) -= 7/3 equals " << (r1 -= r2) << std::endl;
    std::cout << "r1 (new) *= 7/3 equals " << (r1 *= r2) << std::endl;
    std::cout << "r1 (new) /= 7/3 equals " << (r1 /= r2) << std::endl;

    std::cout << "-(7/3) = " << -r2 << std::endl;
    std::cout << "!(7/3) = " << !r2 << std::endl;

    std::cout << "Casting 7/3 to int: " << (int)r2 << std::endl;
    std::cout << "Casting 7/3 to double: " << (double)r2 << std::endl;

    std::cout << Rational(1,3) << std::endl;
}
