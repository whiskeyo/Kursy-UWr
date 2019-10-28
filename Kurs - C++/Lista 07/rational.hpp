#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <iostream>
#include <climits>
#include <vector>

namespace calculations
{

	class RationalException     : std::exception {};
	class DivideByZero          : std::exception {};
	class OutOfRange            : std::exception {};
	class DenominatorEqualsZero : std::exception {};

    int GCD (int a, int b);

	class Rational
	{
		public:
			int nominator;
			int denominator;
            void normalize();

			Rational (int num, int den);    // create num/den
			Rational (int num);			    // create num/1

			friend Rational operator - (Rational a);				            // a becomes -a
			friend Rational operator ! (Rational a);				            // a = p/q becomes q/p
			friend Rational operator + (const Rational &a, const Rational &b);  // a + b
			friend Rational operator - (const Rational &a, const Rational &b);	// a - b
			friend Rational operator * (const Rational &a, const Rational &b);	// a * b
			friend Rational operator / (const Rational &a, const Rational &b);	// a / b

            Rational& operator += (const Rational &x); // this += x equals this = this + x
			Rational& operator -= (const Rational &x); // this -= x equals this = this - x
			Rational& operator *= (const Rational &x); // this *= x equals this = this * x
			Rational& operator /= (const Rational &x); // this /= x equals this = this / x

			operator double();			// casting rational to double, e.g. 1/2 becomes 0.5
			operator int();	// casting rational to int (?)

            friend std::ostream& operator << (std::ostream &output, const Rational &x);
	};
}

#endif // RATIONAL_HPP