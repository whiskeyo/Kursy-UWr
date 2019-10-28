#include "rational.hpp"

namespace calculations
{
    int GCD (int a, int b)
    {
        int remainder = a % b;

        if (remainder == 0)
            return b;
        
        return GCD(b, remainder);
    }

    void Rational::normalize()
    {
        int Tnominator   = this->nominator;
        int Tdenominator = this->denominator;
        int gcd = GCD(Tnominator, Tdenominator);

        Tnominator   = Tnominator   / gcd;
        Tdenominator = Tdenominator / gcd;

        if (Tdenominator < 0)
        {
            Tnominator   *= (-1);
            Tdenominator *= (-1);
        }

        this->nominator   = Tnominator;
        this->denominator = Tdenominator;
    }

    Rational::Rational (int num, int den) // create num/den
    {
        if (den == 0)
            throw DenominatorEqualsZero();

        this->nominator   = num;
        this->denominator = den;
        normalize();
    }

    Rational::Rational (int num) // create num/1
    {
        this->nominator   = num;
        this->denominator = 1;
    }

    Rational operator - (Rational a) // a becomes -a
    {
        a.nominator *= (-1);
        return a;
    }

    Rational operator ! (Rational a) // a = p/q becomes q/p
    {
        if (a.nominator == 0)
            throw DenominatorEqualsZero();
        
        int temp = a.nominator;
        a.nominator = a.denominator;
        a.denominator = temp;
        a.normalize();

        return a;
    }

    Rational operator + (const Rational &a, const Rational &b) // a + b
    {
        long long int tempN = a.nominator * b.denominator + b.nominator * a.denominator;
        long long int tempD = a.denominator * b.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        Rational result(a.nominator * b.denominator + b.nominator * a.denominator, 
                        a.denominator * b.denominator);
        result.normalize();

        return result;
    }

    Rational operator - (const Rational &a, const Rational &b) // a - b
    {
        long long int tempN = a.nominator * b.denominator - b.nominator * a.denominator;
        long long int tempD = a.denominator * b.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        Rational result(a.nominator * b.denominator - b.nominator * a.denominator, 
                        a.denominator * b.denominator);
        //result.normalize();

        return result;
    }

    Rational operator * (const Rational &a, const Rational &b) // a * b
    {
        long long int tempN = a.nominator * b.nominator;
        long long int tempD = a.denominator * b.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        Rational result(a.nominator * b.nominator, a.denominator * b.denominator);
        result.normalize();

        return result;
    }

    Rational operator / (const Rational &a, const Rational &b) // a / b
    {
        if (b.nominator == 0)
            throw DivideByZero();

        long long int tempN = a.nominator * b.denominator;
        long long int tempD = a.denominator * b.nominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        Rational result(a.nominator * b.denominator, a.denominator * b.nominator);
        result.normalize();

        return result;
    }

    
    Rational& Rational::operator += (const Rational &x) // this += x equals this = this + x
    {
        long long int tempN = this->nominator * x.denominator + x.nominator * this->denominator;
        long long int tempD = this->denominator * x.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        this->nominator   = this->nominator * x.denominator + x.nominator * this->denominator;
        this->denominator = this->denominator * x.denominator;
        normalize();

        return *this;
    }

    Rational& Rational::operator -= (const Rational &x) // this -= x equals this = this - x
    {
        long long int tempN = this->nominator * x.denominator - x.nominator * this->denominator;
        long long int tempD = this->denominator * x.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        this->nominator   = this->nominator * x.denominator - x.nominator * this->denominator;
        this->denominator = this->denominator * x.denominator;
        normalize();

        return *this;
    }

    Rational& Rational::operator *= (const Rational &x) // this *= x equals this = this * x
    {
        long long int tempN = this->nominator   * x.nominator;
        long long int tempD = this->denominator * x.denominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        this->nominator   = this->nominator   * x.nominator;
        this->denominator = this->denominator * x.denominator;
        normalize();

        return *this;
    }

    Rational& Rational::operator /= (const Rational &x) // this /= x equals this = this / x
    {
        if (x.nominator == 0)
            throw DivideByZero();

        long long int tempN = this->nominator   * x.denominator;
        long long int tempD = this->denominator * x.nominator;
        if (tempN > INT_MAX || tempN < INT_MIN || tempD > INT_MAX || tempD < INT_MIN)
            throw OutOfRange();

        this->nominator   = this->nominator   * x.denominator;
        this->denominator = this->denominator * x.nominator;
        normalize();

        return *this;
    }
    
    Rational::operator double()
    {
        return (double) this->nominator / this->denominator;
    }

    Rational::operator int()
    {
        return this->nominator / this->denominator;
    }

    std::ostream& operator << (std::ostream &output, const calculations::Rational &x)
    {
        std::vector <int> remainders;
        std::vector <int> results;

        unsigned int i, j;
        int remainder;

        remainder = x.nominator % x.denominator;
        if (remainder == 0)
        {
            output << x.nominator / x.denominator;
            return output;
        }

        if (remainder < 0)
            remainder *= (-1);

        remainders.push_back (remainder);
        
        output << x.nominator / x.denominator;

        while(true)
        {
            results.push_back (remainder * 10 / x.denominator); // decimal numbers
            remainder = remainder * 10 % x.denominator;
            
            if (remainder == 0) // number does not have an infinite decimal expansion
            {
                output << ".";
                for (i = 0; i < results.size(); i++)
                    output << results[i];
                return output;
            }

            for (j = 0; j < remainders.size(); j++) // decimal expansion starts to repeat
            {
                if (remainder == remainders[j])
                {
                    output << ".";
                    for (i = 0; i < j; i++)
                        output << results[i];

                    output << "(";
                    for (i = j; i < results.size(); i++)
                        output << results[i];
                    output << ")";

                    return output;
                }
            }
            remainders.push_back (remainder);
        }
    }
}