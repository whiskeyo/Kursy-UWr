#include <iostream>
#include <exception>    // std::invalid_argument, std::out_of_range
#include <string>
#include <cmath>        // sqrt()

int main()
{
    std::string Area;
    std::clog << "Podaj pole powierzchni: "; std::cin >> Area;

    double area;

    try { area = std::stod(Area); }
    catch (std::invalid_argument&)
    {
        std::cerr << "Nieprawidlowy argument.\n";
        exit (-1);
    }
    catch (std::out_of_range&)
    {
        std::cerr << "Argument wykracza poza zakres double.\n";
        exit (-1);
    }

    if (area < 0)
    {
        std::cerr << "Podano ujemna wartosc powierzchni.";
        exit (-1);
    }

    double r = sqrt(area / M_PI);

    std::cout << "Promien = " << r << std::endl;
}
