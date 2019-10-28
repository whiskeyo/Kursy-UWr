#include <iostream>
#include <cmath>
#include "point.hpp"
#include "linesegment.hpp"
#include "triangle.hpp"
#include "extrafunctions.hpp"

int main()
{
    std::cout << "Tests of a point class: " << std::endl;
    Point x(5.21, 6.31);
    Point y;
    std::cout << "At the beginning: " << x << " and " << y << std::endl;
    x.translate(1.21, -4.31);
    std::cout << "After translation: " << x << std::endl;
    x.rotate(0, 0, M_PI);
    std::cout << "After rotation (based on x,y coordinates): " << x << std:: endl;
    x.rotate(Point(), M_PI);
    std::cout << "After rotation (based on a point): " << x << std:: endl;

    std::cout << std::endl << "Tests of a line segment class: " << std::endl;
    LineSegment a, a1, a2;
    LineSegment b(0, 1, 5, 6);
    LineSegment c(Point(0,3), Point(3,6));
    LineSegment d = b;
    std::cout << a << "\n" << b << "\n" << c << "\n" << d << std::endl;
    b.translate(5.3, 3.1);
    std::cout << "After translation: " << b << std::endl;
    a1.rotate(0, 0, M_PI);
    std::cout << "After rotation (based on x,y coordinates): " << a1 << std:: endl;
    a2.rotate(Point(), M_PI);
    std::cout << "After rotation (based on a point): " << a2 << std:: endl;
    std::cout << "Length of line 'a' = " << a.length() << std::endl;
    Point f = a.middle();
    std::cout << "Middle of a segment " << a << " is " << f << std::endl;

    std::cout << std::endl << "Tests of a triangle class: " << std::endl;
    Triangle tri1(0, 0, 1, 1, 3, 5);
    Triangle tri2(Point(0,0), Point(1,3), Point(3,1));
    std::cout << "First triangle: " << tri1 << std::endl
              << "Second triangle: " << tri2 << std::endl;
    std::cout << "Area of the first triangle: " << tri1.area() << std::endl
              << "Area of the second triangle: " << tri2.area() << std::endl;
    std::cout << "Perimeter of the first triangle: " << tri1.perimeter() << std::endl
              << "Perimeter of the second triangle: " << tri2.perimeter() << std::endl;
    Point tri_test(0.9, 1.3);
    std::cout << "Point " << tri_test << " is " << (tri1.isInside(tri_test) ? "inside" : "outside") << " the triangle." << std::endl;
    std::cout << "The middle point of the first triangle is " << tri1.middle() << std::endl;
    tri1.rotate(Point(0,0), M_PI/4);
    std::cout << "First triangle after rotation: " << tri1 << std::endl;
    tri2.translate(5, 6);
    std::cout << "Second triangle after translation: " << tri2 << std::endl;

    std::cout << std::endl << "Test of extra functions: " << std::endl;
    LineSegment par1(Point(0,0), Point(5, 0));
    LineSegment par2(Point(0,5), Point(13, 5));
    std::cout << "Line segments par1 " << par1 << " and par2 " << par2 << (areParallel(par1, par2) ? " are " : " are not ") << "parallel." << std::endl;
    LineSegment per1 = par1;
    LineSegment per2(Point(2,0), Point(2, 10));
    std::cout << "Line segments per1 " << per1 << " and per2 " << per2 << (arePerpendicular(per1, per2) ? " are " : " are not ") << "perpendicular." << std::endl;

    return 0;
}
