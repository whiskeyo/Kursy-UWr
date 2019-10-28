#include "linesegment.hpp"

LineSegment::LineSegment()
{
    a.setX(0);
    a.setY(0);
    b.setX(1);
    b.setY(1);
}

LineSegment::LineSegment(double x1, double y1, double x2, double y2)
{
    if (x1 == x2 && y1 == y2)
        throw std::invalid_argument("You cannot create a line with a length 0.\n");

    else
    {
        a.setX(x1);
        a.setY(y1);
        b.setX(x2);
        b.setY(y2);
    }
}

LineSegment::LineSegment(Point c, Point d)
{
    if (c.getX() == d.getX() && c.getY() == d.getY())
        throw std::invalid_argument("You cannot create a line with a length 0.\n");

    else
    {
        a = c;
        b = d;
    }
}

LineSegment::LineSegment(const LineSegment& l)
{
    a = l.a;
    b = l.b;
}

Point LineSegment::getA()
{
    return a;
}

Point LineSegment::getB()
{
    return b;
}

void LineSegment::translate(double dx, double dy)
{
    a.setX( a.getX() + dx );
    a.setY( a.getY() + dy );
    b.setX( b.getX() + dx );
    b.setY( b.getY() + dy );
}

void LineSegment::rotate(double bx, double by, double theta)
{
    a.setX((a.getX() - bx) * cos(theta) - (a.getY() - by) * sin(theta) + bx);
    a.setY((a.getX() - bx) * sin(theta) + (a.getY() - by) * cos(theta) + by);

    b.setX((b.getX() - bx) * cos(theta) - (b.getY() - by) * sin(theta) + bx);
    b.setY((b.getX() - bx) * sin(theta) + (b.getY() - by) * cos(theta) + by);
}

void LineSegment::rotate(Point c, double theta)
{
    a.setX((a.getX() - c.getX()) * cos(theta) - (a.getY() - c.getY()) * sin(theta) + c.getX());
    a.setY((a.getX() - c.getX()) * sin(theta) + (a.getY() - c.getY()) * cos(theta) + c.getY());

    b.setX((b.getX() - c.getX()) * cos(theta) - (b.getY() - c.getY()) * sin(theta) + c.getX());
    b.setY((b.getX() - c.getX()) * sin(theta) + (b.getY() - c.getY()) * cos(theta) + c.getY());
}

double LineSegment::length()
{
    double bracket1 = b.getX() - a.getX();
    double bracket2 = b.getY() - a.getY();

    return sqrt(pow(bracket1, 2) + pow(bracket2, 2));
}

bool LineSegment::doesBelong(Point c)
{

    if ((c.getX() <= b.getX() && c.getY() <= b.getY() && c.getX() >= a.getX() && c.getY() >= a.getY())
        || (c.getX() >= b.getX() && c.getY() >= b.getY() && c.getX() <= a.getX() && c.getY() <= a.getY()))
    // y = mx + i, where m is slope, i is intersection point with OY
    {
        double m = (a.getY() - b.getY()) / (a.getX() - b.getX());
        double i = a.getY() - (a.getY() - b.getY()) / (a.getX() - b.getX()) * a.getX();

        // 1e-10 is a tolerance
        if (std::abs(c.getY() - (m * c.getX() + i)) <= 1e-10)
            return true;
        else
            return false;
    }
    else
        return false;
}

Point LineSegment::middle()
{
    Point c;
    c.setX((a.getX()+b.getX()) / 2);
    c.setY((a.getY()+b.getY()) / 2);

    return c;
}

std::ostream& operator<<(std::ostream& out, const LineSegment& A)
{
    return out << "A = " << A.a << ", B = " << A.b;
}
