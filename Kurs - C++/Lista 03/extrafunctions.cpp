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

bool doIntersect(LineSegment c, LineSegment d) // c (p1,q1), d (p2,q2)
{
    // All points needed:
    Point p1 = c.getA();
    Point q1 = c.getB();
    Point p2 = d.getA();
    Point q2 = d.getB();
    // Four orientations needed for general and special cases:
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special cases
    // p1, q1, p2 are colinear, p2 lies on p1q1
    if (o1 == 0 && c.doesBelong(p2))
        return true;
    // p1, q1, q2 are colinear, q2 lies on p1q1
    if (o2 == 0 && c.doesBelong(q2))
        return true;
    // p2, q2, p1 are colinear, p1 lies on p2q2
    if (o3 == 0 && d.doesBelong(p1))
        return true;
    // p2, q2, q1 are colinear, q1 lies on p2q2
    if (o4 == 0 && d.doesBelong(q1))
        return true;

    return false; // if it does not meet any condition above
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
        return false;

    Point a1 = u.getA();
    Point b1 = u.getB();
    Point c1 = u.getC();
    Point a2 = v.getA();
    Point b2 = v.getB();
    Point c2 = v.getB();

    LineSegment ls1(a1, b1);
    LineSegment ls2(b1, c1);
    LineSegment ls3(a1, c1);
    LineSegment ls4(a2, b2);
    LineSegment ls5(b2, c2);
    LineSegment ls6(a2, c2);

    if (doIntersect(ls1, ls4) || doIntersect(ls1, ls5) || doIntersect(ls1, ls6))
        return true;
    if (doIntersect(ls2, ls4) || doIntersect(ls2, ls5) || doIntersect(ls2, ls6))
        return true;
    if (doIntersect(ls3, ls4) || doIntersect(ls3, ls5) || doIntersect(ls3, ls6))
        return true;

    return false;

}
