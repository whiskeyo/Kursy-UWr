#ifndef LINESEGMENT_HPP
#define LINESEGMENT_HPP

#include <iostream>
#include <cmath>
#include <exception>
#include "point.hpp"

class LineSegment
{
    private:
        Point a;
        Point b;

    public:
        LineSegment();
        LineSegment(double x1, double y1, double x2, double y2);
        LineSegment(Point c, Point d);
        LineSegment(const LineSegment& l);

        Point getA();
        Point getB();

        void translate(double dx, double dy);
        void rotate(double bx, double by, double theta);
        void rotate(Point c, double theta);

        double length();
        bool doesBelong(Point c);
        Point middle();

        friend std::ostream& operator<<(std::ostream& out, const LineSegment& A);
};

#endif //LINESEGMENT_HPP

