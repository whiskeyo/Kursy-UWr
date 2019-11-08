#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include <cmath>
#include <exception>
#include "point.hpp"


class Triangle
{
    private:
        Point a;
        Point b;
        Point c;

        float r_color;
        float g_color;
        float b_color;

    public:
        Triangle();
        Triangle(double x1, double y1, double x2, double y2, double x3, double y3, float rc, float gc, float bc);
        Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
        Triangle(Point d, Point e, Point f);
        //Triangle(const Triangle& t);

        Point getA();
        Point getB();
        Point getC();

        float getRcol() { return r_color; }
        float getGcol() { return g_color; }
        float getBcol() { return b_color; }

        void translate(double dx, double dy);
        void rotate(double bx, double by, double theta);
        void rotate(Point d, double theta);

        double area();
        double perimeter();
        bool isInside(Point d);
        Point middle();

        friend std::ostream& operator<<(std::ostream& out, const Triangle& t);
};

#endif //TRIANGLE_HPP

