#ifndef EXTRAFUNCTIONS_HPP
#define EXTRAFUNCTIONS_HPP

#include <cmath>
#include <exception>
#include <iostream>
#include "point.hpp"
#include "linesegment.hpp"
#include "triangle.hpp"

double distance(Point a, Point b);
double triangleArea(Point a, Point b, Point c); // for Triangle::isInside function
bool areParallel(LineSegment c, LineSegment d);
bool arePerpendicular(LineSegment c, LineSegment d);
int orientation(Point a, Point b, Point c); // for line segment intersection
bool doIntersect(LineSegment c, LineSegment d);
bool isTriangleInsideAnother(Triangle u, Triangle v);
bool doTrianglesIntersect(Triangle u, Triangle v);

#endif //EXTRAFUNCTIONS_HPP
