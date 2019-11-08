#include "point.hpp"

Point::Point()
{
    x = 0;
    y = 0;
}

Point::Point(double val_x, double val_y)
{
    x = val_x;
    y = val_y;
}

Point::Point(const Point& p)
{
    x = p.x;
    y = p.y;
}

void Point::translate(double dx, double dy)
{
    x = x + dx;
    y = y + dy;
}

void Point::rotate(double bx, double by, double theta)
{
    x = (x - bx) * cos(theta) - (y - by) * sin(theta) + bx;
    y = (x - bx) * sin(theta) + (y - by) * cos(theta) + by;
}

void Point::rotate(Point a, double theta)
{
    x = (x - a.x) * cos(theta) - (y - a.y) * sin(theta) + a.x;
    y = (x - a.x) * sin(theta) + (y - a.y) * cos(theta) + a.y;
}

void Point::setX(double val)
{
    x = val;
}

void Point::setY(double val)
{
    y = val;
}

double Point::getX()
{
    return x;
}

double Point::getY()
{
    return y;
}

std::ostream& operator<<(std::ostream& out, const Point& A)
{
    return out << "(" << A.x << ", " << A.y << ")";
}
