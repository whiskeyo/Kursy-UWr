#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>
#include <cmath>

class Point
{
    private:
        double x;
        double y;

    public:
        Point();
        Point(double val_x, double val_y);
        Point(const Point& p);

        void translate(double dx, double dy);
        void rotate(double bx, double by, double theta);
        void rotate(Point a, double theta);

        void setX(double val);
        void setY(double val);
        double getX();
        double getY();

        friend std::ostream& operator<<(std::ostream& out, const Point& A);
};

#endif //POINT_HPP
