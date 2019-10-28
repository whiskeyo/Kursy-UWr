#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <initializer_list>
#include <iostream>

class Polynomial
{
    private:
        int n;      // degree of polynomial
        double* a;  // array of coefficients

    public:
        Polynomial(int degree = 0, double coefficient = 1.0);   // ctor creating monomial
        Polynomial(int degree, const double coefficient[]);     // ctor creating polynomial (using array)
        Polynomial(std::initializer_list<double> coefficient);  // ctor creating polynomial (using init list)
        Polynomial(const Polynomial& polynomial);               // copying ctor
        Polynomial(Polynomial&& polynomial);                    // moving ctor
        Polynomial& operator = (const Polynomial& polynomial);  // copying assignment
        Polynomial& operator = (Polynomial&& polynomial);       // moving assignment
        
        ~Polynomial();                                          // destructor

        friend std::istream& operator >> (std::istream& input, Polynomial& polynomial);       // input stream overload
        friend std::ostream& operator << (std::ostream& output, Polynomial& polynomial);      // output stream overload

        friend Polynomial operator + (const Polynomial& p1, const Polynomial& p2);
        friend Polynomial operator - (const Polynomial& p1, const Polynomial& p2);
        friend Polynomial operator * (const Polynomial& p1, const Polynomial& p2);
        friend Polynomial operator * (double c);

        Polynomial& operator += (const Polynomial& polynomial);
        Polynomial& operator -= (const Polynomial& polynomial);
        Polynomial& operator *= (const Polynomial& polynomial);
        Polynomial& operator *= (double c);

        double operator () (double x) const;    // count value of a polynomial for given x
        double operator [] (int i) const;       // show i-th coefficient
};

#endif