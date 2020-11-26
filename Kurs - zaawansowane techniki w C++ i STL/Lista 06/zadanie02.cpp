#include <iostream>
#include <cstdint>
#include <string>
#include <list>
#include <algorithm>

enum class quarter { first, second, third, fourth, none };

class point {
private:
    int x;
    int y;
    int r;
    int g;
    int b;
    std::string name;

public:
    point (int x, int y, int r, int g, int b, const std::string& name)
        : x(x), y(y), r(r), g(g), b(b), name(name) {}

    // getters & setters
    int     get_x () const { return x; }
    int     get_y () const { return y; }
    int     get_r () const { return r; }  
    int     get_g () const { return g; }  
    int     get_b () const { return b; }  

    void    set_x (int value) { x = value; }
    void    set_y (int value) { y = value; }
    void    set_r (int value) { r = value; }
    void    set_g (int value) { g = value; }
    void    set_b (int value) { b = value; }

    std::string get_name () const            { return name; }
    void        set_name (std::string value) { name = value; }

    // other functions
    double calculate_brightness () const {
        return 0.3 * r + 0.59 * g + 0.11 * b;
    }

    quarter which_quarter () const {
        if (x > 0 && y > 0) return quarter::first;
        if (x < 0 && y > 0) return quarter::second;
        if (x < 0 && y < 0) return quarter::third;
        if (x > 0 && y < 0) return quarter::fourth;

        return quarter::none; // point is on the line
    }

    void print() {
        std::cout << x << "\t" << y << "\t" << r << "\t" << g << "\t" << b << "\t" << name << std::endl; 
    }
};

void print_points(const std::string& info, const std::list<point>& points) {
    std::cout << info << std::endl;
    for (auto p : points)
        p.print();

    std::cout << std::endl;
}

int main() {
    std::list<point> points = {
        point ( 3,  3, 123,  61, 217, "adssad"),
        point ( 1,  2, 0,     0,  22, "adsd"),
        point ( 2, -7, 136, 161, 231, "adss"),
        point ( 0, -6, 231,  41, 172, "bga"),
        point (-1,  5, 196,  54,  15, "usaousad"),
        point (-2,  7, 175,  87,   1, "paase"),
        point (-3,  3, 16,   14,  61, "pnr"),
        point (-4, -3, 12,  100, 102, "nad"),
        point (-6, -1, 174, 251, 107, "oaspoase"),
        point (-8,  3, 161, 253, 177, "dsa"),
        point (-8,  3, 145,  12, 184, "1dddd13"),
        point ( 5,  3, 133,  55, 201, "dy"),
        point ( 7, -3, 179, 196, 100, "ppdsaspa"),
        point ( 6, -6, 112, 201,  10, "sianeiosae"),
        point ( 9,  3, 177,  65,   1, "a"),
        point (-3,  2, 17,   65,  11, "dasd saoeese"),
        point (13,  1, 127,  17,  23, "n123ip213n")
    };

    print_points("Starting points", points);

    // delete points if their name is longer than 5 chars
    auto name_longer_than_5 = [](const point& p) -> bool {
        return p.get_name().length() > 5;
    };

    points.remove_if(name_longer_than_5);
    print_points("After first removal (of names longer than 5 chars)", points);

    // count how many points there are in each quarter
    unsigned int first = 0, second = 0, third = 0, fourth = 0, none = 0;
    for (auto& p : points) {
        quarter q = p.which_quarter();
        switch (q) {
            case quarter::first:  first++;  break;
            case quarter::second: second++; break;
            case quarter::third:  third++;  break;
            case quarter::fourth: fourth++; break;
            case quarter::none:   none++;   break;
        }
    }
    std::cout << "Points in first quarter: "  << first  << std::endl;
    std::cout << "Points in second quarter: " << second << std::endl;
    std::cout << "Points in third quarter: "  << third  << std::endl;
    std::cout << "Points in fourth quarter: " << fourth << std::endl;
    std::cout << "Points lying on any line: " << none   << std::endl << std::endl;


    // sort by brightness / luminance
    auto sort_brightness = [](const point& a, const point& b) {
        return a.calculate_brightness() < b.calculate_brightness(); 
    };

    points.sort(sort_brightness);
    print_points("After brightness sort", points);

    // count how many dark points there are (brightness < 64)
    // move them to another list and print them
    std::list<point> dark_points;

    auto is_dark = [](const point& p) {
        return p.calculate_brightness() < 64;
    };

    for (auto& p : points)
        if (is_dark(p))
            dark_points.push_back(p);

    print_points("Dark points", dark_points);
}