#include <iostream>
#include <iomanip>      // std::setprecision()
#include <exception>    // std::invalid_argument, std::out_of_range
#include <cmath>        // sqrt()

int main(int argc, char *argv[])
{
    std::string aStr, bStr, cStr;

    std::clog << "Podaj dlugosc boku a: "; std::cin >> aStr;
    std::clog << "Podaj dlugosc boku b: "; std::cin >> bStr;
    std::clog << "Podaj dlugosc boku c: "; std::cin >> cStr;

    double a, b, c;

    try
    {
        a = std::stod(aStr);
        b = std::stod(bStr);
        c = std::stod(cStr);
    }
    catch (std::invalid_argument&)
    {
        std::cerr << "Nieprawidlowy argument.\n";
        return -1;
    }
    catch (std::out_of_range&)
    {
        std::cerr << "Argument wykracza poza zakres double.\n";
        return -1;
    }

    if (a < 0 || b < 0 || c < 0 || a + b < c || b + c < a || a + c < b)
    {
        std::cerr << "Nieprawidlowa dlugosc bokow lub trojkat nie istnieje.\n";
        exit(-1);
    }

    double L = (a + b + c) / 2;
    double Area = sqrt(L * (L - a) * (L - b) * (L - c));

    std::cout << std::setprecision(4) << "\nPole = " << Area << std::endl;
}
