#include "polynomial.hpp"
#include <initializer_list>
#include <iostream>

Polynomial::Polynomial(int degree = 0, double coefficient = 1.0)
{
    n = degree;
    a = new double[n + 1];

    a[0] = coefficient;
}

Polynomial::Polynomial(int degree, const double coefficient[])
{
    n = degree;
    a = new double[n + 1];

    for (int i = 0; i < coefficient.size(); i++)
        a[i] = coefficient[i];
}

Polynomial::Polynomial(std::initializer_list<double> coefficient)
{
    n = coefficient.size();
    a = new double[n + 1];
    
    for (auto x : coefficient)
        a[x] = coefficient[x];

}

Polynomial::Polynomial(const Polynomial& polynomial)
{
    n = polynomial.n;
    a = polynomial.a;
}

Polynomial::Polynomial(Polynomial&& polynomial)
{
    n = polynomial.n;
    a = polynomial.a;
    polynomial.a = nullptr;
}

Polynomial & Polynomial::operator = (const Polynomial& polynomial)
{
    delete [] a;
    n = polynomial.n;
    a = polynomial.a;

    return *this;
}

Polynomial & Polynomial::operator = (Polynomial&& polynomial)
{
    delete [] a;
    n = polynomial.n;
    a = polynomial.a;
    polynomial.a = nullptr;

    return *this;
}

Polynomial::~Polynomial()
{
    delete [] a;
}

std::ostream& operator << (std::ostream& output, Polynomial& polynomial)
{
    std::string out;
    int degree;

    for (int i = polynomial.n; i >= 0; i--)
    {
            out += polynomial.a[i] + "x^" + i + "\n";
    }

    return out;
}

std::ostream& operator >> (std::istream& input, Polynomial& polynomial)
{
    
}

Polynomial operator + (const Polynomial& p1, const Polynomial& p2)
{
    if (p1.n < p2.n)
    {
        Polynomial temp;
        temp = p1;
        p1 = p2;
        p2 = temp;
    }

    
}

Polynomial operator - (const Polynomial& p1, const Polynomial& p2)
{

}

Polynomial operator * (const Polynomial& p1, const Polynomial& p2)
{

}

Polynomial operator (double c)
{

}