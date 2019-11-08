#include "extrafunctions.hpp"

double distance(Point a, Point b)
{
    double bracket1 = b.getX() - a.getX();
    double bracket2 = b.getY() - a.getY();

    return sqrt(pow(bracket1, 2) + pow(bracket2, 2));
}

double triangleArea(Point a, Point b, Point c)
{
    double A = distance(a, b);
    double B = distance(b, c);
    double C = distance(a, c);
    double x = (A + B + C) / 2;

    return sqrt(x * (x - A) * (x - B) * (x - C));
}

bool areParallel(LineSegment c, LineSegment d)
{
    // y = mx + i, where m is slope, i is interception point with OY (unnecessary)
    double m1 = (c.getA().getY() - c.getB().getY()) / (c.getA().getX() - c.getB().getX());
    double m2 = (d.getA().getY() - d.getB().getY()) / (d.getA().getX() - d.getB().getX());

    if (m1 == m2)
        return true;
    else
        return false;
}

bool arePerpendicular(LineSegment c, LineSegment d)
{
    double m1 = (c.getA().getY() - c.getB().getY()) / (c.getA().getX() - c.getB().getX());
    double m2 = (d.getA().getY() - d.getB().getY()) / (d.getA().getX() - d.getB().getX());

    if (m1 == (-1 / m2))
        return true;
    else
        return false;
}

int orientation(Point a, Point b, Point c)
{
    int value = (b.getY() - a.getY()) * (c.getX() - b.getX()) - (b.getX() - a.getX()) * (c.getY() - b.getY());

    if (value == 0)
        return 0; // so it is colinear
    else if (value > 0)
        return 1; // clockwise
    else
        return 2; // counter-clockwise
}

bool doIntersect(LineSegment c, LineSegment d)
{
    // http://www.cs.swan.ac.uk/~cssimon/line_intersection.html
    double denom = ((d.getB().getX()-d.getA().getX())*(c.getA().getY()-c.getB().getY())-(c.getA().getX()-c.getB().getX())*(d.getB().getY()-d.getA().getY()));
    if (denom == 0)
        return false;

    double ta = ((d.getA().getY()-d.getB().getY())*(c.getA().getX()-d.getA().getX())+(d.getB().getX()-d.getA().getX())*(c.getA().getY()-d.getA().getY())) / denom;       
    double tb = ((c.getA().getY()-c.getB().getY())*(c.getA().getX()-d.getA().getX())+(c.getB().getX()-c.getA().getX())*(c.getA().getY()-d.getA().getY())) / denom;

    if (0 <= ta && ta <= 1 && 0 <= tb && tb <= 1)
        return true;
    else 
        return false;
}

bool isTriangleInsideAnother(Triangle u, Triangle v)
{
    Point a1 = u.getA();
    Point b1 = u.getB();
    Point c1 = u.getC();
    Point a2 = v.getA();
    Point b2 = v.getB();
    Point c2 = v.getB();

    if (u.isInside(a2) && u.isInside(b2) && u.isInside(c2))
        return true;
    else if (v.isInside(a1) && v.isInside(b1) && v.isInside(c1))
        return true;
    else
        return false;
}

bool doTrianglesIntersect(Triangle u, Triangle v)
{
    if (isTriangleInsideAnother(u, v)) // so triangles do not intersect
        return true;

    Point a1 = u.getA();
    Point b1 = u.getB();
    Point c1 = u.getC();
    Point a2 = v.getA();
    Point b2 = v.getB();
    Point c2 = v.getC();

    LineSegment ls1(a1, b1);
    LineSegment ls2(b1, c1);
    LineSegment ls3(c1, a1);
    LineSegment ls4(a2, b2);
    LineSegment ls5(b2, c2);
    LineSegment ls6(c2, a2);

    if (doIntersect(ls1, ls4) || doIntersect(ls1, ls5) || doIntersect(ls1, ls6) || 
        doIntersect(ls2, ls4) || doIntersect(ls2, ls5) || doIntersect(ls2, ls6) ||
        doIntersect(ls3, ls4) || doIntersect(ls3, ls5) || doIntersect(ls3, ls6))
        return true;

    return false;
}
