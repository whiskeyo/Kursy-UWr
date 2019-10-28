#include "triangle.hpp"
#include "extrafunctions.hpp"

Triangle::Triangle()
{
    a.setX(0);
    a.setY(0);
    b.setX(1);
    b.setY(0);
    c.setX(1);
    c.setY(1);
}

Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (x1 == x2 && x2 == x3 && y1 == y2 && y2 == y3)
        throw std::invalid_argument("You cannot create a triangle with sides of a length 0.\n");

    // m1 - segment AB, m2 - segment BC
    double m1 = (y1 - y2) / (x1 - x2);
    double m2 = (y2 - y3) / (x2 - x3);

    if (m1 == m2)
        throw std::invalid_argument("All the points are collinear.\n");

    else
    {
        a.setX(x1);
        a.setY(y1);
        b.setX(x2);
        b.setY(y2);
        c.setX(x3);
        c.setY(y3);
    }
}

Triangle::Triangle(Point d, Point e, Point f)
{
    if (d.getX() == e.getX() && e.getX() == f.getX() && d.getY() == e.getY() && e.getY() == f.getY())
        throw std::invalid_argument("You cannot create a triangle with sides of a length 0.\n");

    // m1 - segment AB, m2 - segment BC
    double m1 = (d.getY() - e.getY()) / (d.getX() - e.getX());
    double m2 = (e.getY() - f.getY()) / (e.getX() - f.getX());

    if (m1 == m2)
        throw std::invalid_argument("All the points are collinear.\n");

    else
    {
        a = d;
        b = e;
        c = f;
    }
}

Triangle::Triangle(const Triangle& t)
{
    a = t.a;
    b = t.b;
    c = t.c;
}

Point Triangle::getA()
{
    return a;
}

Point Triangle::getB()
{
    return b;
}

Point Triangle::getC()
{
    return c;
}

void Triangle::translate(double dx, double dy)
{
    a.setX( a.getX() + dx );
    a.setY( a.getY() + dy );
    b.setX( b.getX() + dx );
    b.setY( b.getY() + dy );
    c.setX( c.getX() + dx );
    c.setY( c.getY() + dy );
}

void Triangle::rotate(double bx, double by, double theta)
{
    a.setX((a.getX() - bx) * cos(theta) - (a.getY() - by) * sin(theta) + bx);
    a.setY((a.getX() - bx) * sin(theta) + (a.getY() - by) * cos(theta) + by);

    b.setX((b.getX() - bx) * cos(theta) - (b.getY() - by) * sin(theta) + bx);
    b.setY((b.getX() - bx) * sin(theta) + (b.getY() - by) * cos(theta) + by);

    c.setX((c.getX() - bx) * cos(theta) - (c.getY() - by) * sin(theta) + bx);
    c.setY((c.getX() - bx) * sin(theta) + (c.getY() - by) * cos(theta) + by);
}

void Triangle::rotate(Point d, double theta)
{
    a.setX((a.getX() - d.getX()) * cos(theta) - (a.getY() - d.getY()) * sin(theta) + d.getX());
    a.setY((a.getX() - d.getX()) * sin(theta) + (a.getY() - d.getY()) * cos(theta) + d.getY());

    b.setX((b.getX() - d.getX()) * cos(theta) - (b.getY() - d.getY()) * sin(theta) + d.getX());
    b.setY((b.getX() - d.getX()) * sin(theta) + (b.getY() - d.getY()) * cos(theta) + d.getY());

    c.setX((c.getX() - d.getX()) * cos(theta) - (c.getY() - d.getY()) * sin(theta) + d.getX());
    c.setY((c.getX() - d.getX()) * sin(theta) + (c.getY() - d.getY()) * cos(theta) + d.getY());
}

double Triangle::area()
{
    double x = perimeter() / 2;
    double A = distance(a, b);
    double B = distance(b, c);
    double C = distance(a, c);

    return sqrt(x * (x - A) * (x - B) * (x - C));
}

double Triangle::perimeter()
{
    return distance(a, b) + distance(b, c) + distance(a, c);
}

bool Triangle::isInside(Point d)
{
    // In order to check whether a point lies inside a triangle,
    // we calculate areas of triangles formed by ABD, ACD, BCD
    // and check if their sum is equal the area of ABC
    double A = area();
    double A1 = triangleArea(a, b, d);
    double A2 = triangleArea(a, c, d);
    double A3 = triangleArea(b, c, d);

    if (std::abs(A - (A1 + A2 + A3)) < 1e-10)
        return true;
    else
        return false;
}

Point Triangle::middle()
{
    double x = (a.getX() + b.getX() + c.getX()) / 3;
    double y = (a.getY() + b.getY() + c.getY()) / 3;

    return Point(x,y);
}

std::ostream& operator<<(std::ostream& out, const Triangle& t)
{
    return out << "A = " << t.a << ", B = " << t.b << ", C = " << t.c;
}
