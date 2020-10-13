#include <iostream>
#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>

struct Point
{
    int32_t x;
    int32_t y;
    Point() {}
    Point(int32_t lx, int32_t ly) : x(lx), y(ly) {}
};

inline uint64_t pow2(int32_t x);
inline Point findMiddle(const Point& p1, const Point& p2);
inline double findDistance(const Point& p1, const Point& p2);
inline double findPerimeter(const Point& p1, const Point& p2, const Point& p3);
inline bool compareX(const Point& p1, const Point& p2);
inline bool compareY(const Point& p1, const Point& p2);
void prepareDataAndSolve(std::vector<Point>& points);
void solve(uint32_t n, const Point points[], const std::vector<Point>& m_sorted_y);
void bruteForce(const std::vector<Point>& p);

Point a, b, c; // punkty wynikowe przechowuje w zmiennych globalnych dla ulatwienia
const double UNOBTAINABLE = std::numeric_limits<double>::max();
const double MAX_RANGE    = 1e7;
double MIN_DIST = UNOBTAINABLE;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector<Point> m_points;
    uint32_t n;
    std::cin >> n;
    m_points.reserve(n);
    
    for (uint32_t i = 0; i < n; i++)
    {   
        int32_t x, y;
        std::cin >> x >> y;
        m_points.push_back(Point(x, y));
    }

    a = m_points[0];
    b = m_points[1];
    c = m_points[2];

    if (n < 10)
        bruteForce(m_points);
    else
        prepareDataAndSolve(m_points);

    std::cout << a.x << " " << a.y << '\n';
    std::cout << b.x << " " << b.y << '\n';
    std::cout << c.x << " " << c.y;
}

inline uint64_t pow2(int32_t x) // uint64_t zeby nie wyjsc poza zakres
{ 
    return uint64_t(x) * uint64_t(x); 
}

inline Point findMiddle(const Point& p1, const Point& p2)
{
    return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

inline double findDistance(const Point& p1, const Point& p2)
{
    return sqrt((double)(pow2(p1.x - p2.x) + pow2(p1.y - p2.y)));
}

inline double findPerimeter(const Point& p1, const Point& p2, const Point& p3)
{
    return findDistance(p1, p2) + findDistance(p2, p3) + findDistance(p3, p1);
}

inline bool compareX(const Point& p1, const Point& p2)
{
    if (p1.x != p2.x)
        return p1.x < p2.x;
    else
        return p1.y < p2.y;
}

inline bool compareY(const Point& p1, const Point& p2)
{
    if (p1.y != p2.y)
        return p1.y < p2.y;
    else
        return p1.x < p2.x;
}

void prepareDataAndSolve(std::vector<Point>& l_points)
{
    std::sort(
        l_points.begin(), 
        l_points.end(), 
        [](const Point& p1, const Point& p2) -> bool
        {
            if (p1.x != p2.x)
                return p1.x < p2.x;
            else
                return p1.y < p2.y;
        }
    );

    std::vector<Point> m_sorted_y = l_points;
    
    std::sort(
        m_sorted_y.begin(), 
        m_sorted_y.end(), 
        [](const Point& p1, const Point& p2) -> bool
        {
            if (p1.y != p2.y)
                return p1.y < p2.y;
            else
                return p1.x < p2.x;
        }
    );

    solve(l_points.size(), &l_points[0], m_sorted_y);
}

void solve(uint32_t n, const Point l_points[], const std::vector<Point>& l_sorted_y)
{
    if (n < 3) // za malo punktow do odniesienia sie w rekurencji
        return;
    
    uint32_t m_left = n / 2 + 1;        
    uint32_t m_right = n - m_left;

    Point m_mid_point = findMiddle(l_points[m_left], l_points[m_left + 1]);
    std::vector<Point> m_left_sorted_y;  m_left_sorted_y.reserve(m_left);
    std::vector<Point> m_right_sorted_y; m_right_sorted_y.reserve(m_right);

    for (uint32_t i = 0; i < n; i++)
    {
        if (compareX(l_sorted_y[i], m_mid_point))
            m_left_sorted_y.push_back(l_sorted_y[i]);
        else
            m_right_sorted_y.push_back(l_sorted_y[i]);
    }

    MIN_DIST = findPerimeter(a, b, c);
    //MIN_DIST = std::min(MIN_DIST, solve(m_left, l_points, m_left_sorted_y));
    //MIN_DIST = std::min(MIN_DIST, solve(m_right, l_points + m_left, m_right_sorted_y));
    solve(m_left, l_points, m_left_sorted_y);
    solve(m_right, l_points + m_left, m_right_sorted_y);

    static std::vector<Point> m_near_the_line;
    int32_t m_range;
    if (MIN_DIST > UNOBTAINABLE)
        m_range = 2 * MAX_RANGE; // 2 * MAX_RANGE to maksymalny zakres wspolrzednych
    else 
        m_range = MIN_DIST / 2;

    m_near_the_line.clear();
    m_near_the_line.reserve(n);

    uint32_t m_counter = 0;
    for (uint32_t k = 0; k < n; k++)
    {
        Point m_temp = l_sorted_y[k];
        
        if (std::abs(m_temp.x - m_mid_point.x) > m_range)
            continue;

        while (m_counter < m_near_the_line.size() && m_temp.y - m_near_the_line[m_counter].y > m_range)
            m_counter++;

        for (uint32_t i = m_counter; i < m_near_the_line.size(); i++)
            for(uint32_t j = i + 1; j < m_near_the_line.size(); j++)
                if (MIN_DIST > findPerimeter(m_temp, m_near_the_line[i], m_near_the_line[j]))
                {
                    MIN_DIST = findPerimeter(m_temp, m_near_the_line[i], m_near_the_line[j]);
                    a = m_temp;
                    b = m_near_the_line[i];
                    c = m_near_the_line[j]; 
                }
        
        m_near_the_line.push_back(m_temp);
    }
}

void bruteForce(const std::vector<Point>& p)
{
    MIN_DIST = findPerimeter(a, b, c);
    uint32_t size = p.size();
    for (uint32_t i = 0; i < size - 2; i++)
        for (uint32_t j = i + 1; j < size - 1; j++)
            for (uint32_t k = j + 1; k < size; k++)
            {
                double dist = findPerimeter(p[i], p[j], p[k]);
                if (dist < MIN_DIST)
                {
                    MIN_DIST = dist;
                    a = p[i];
                    b = p[j];
                    c = p[k];
                }
            }
}