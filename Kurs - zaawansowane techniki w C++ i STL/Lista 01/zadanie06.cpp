#include <iostream>
#include <cmath>
#include <complex>

// Biorac pod uwage niedokladnosc obliczen zmiennoprzecinkowych uwzgledniam
// przypadek, w ktorym delta jest bardzo bliska zeru po wykonaniu obliczen,
// wykorzystuje do tego mala wartosc epsilona.

constexpr double epsilon = 0.0000001;

bool is_close_to_zero (double value)
{
    return (value > -epsilon) && (value < epsilon);
}

void solve_quadratic_equation (double a, double b, double c)
{
    if (double delta = std::pow(b, 2) - 4.0 * a * c; is_close_to_zero(delta))
    {
        double x = -b / (2.0 * a);
        std::cout << "x = " << x << std::endl;
    }
    else if (delta > 0)
    {
        double x0 = (-b + std::sqrt(delta)) / (2.0 * a);
        double x1 = c / a * x0;
        std::cout << "x0 = " << x0 << ", x1 = " << x1 << std::endl;
    }
    else
    {
        using namespace std::complex_literals; // użycie 1i (jednostka urojona)

        std::complex<double> imaginary = std::sqrt(-delta) * 1i / (2.0 * a);
        double real = -b / (2.0 * a);

        std::complex<double> x0 = real + imaginary;
        std::complex<double> x1 = real - imaginary;

        std::cout << "x0 = " << x0 << ", x1 = " << x1 << std::endl;
    }
}

int main()
{
    std::cout << "f(x) = x^2 + 2x - 3" << std::endl;
    solve_quadratic_equation(1, 2, -3);

    std::cout << "f(x) = x^2 + 4x + 5" << std::endl;
    solve_quadratic_equation(1, 4, 5);

    std::cout << "f(x) = x^2 - 2x + 1" << std::endl;
    solve_quadratic_equation(1, -2, 1);
}