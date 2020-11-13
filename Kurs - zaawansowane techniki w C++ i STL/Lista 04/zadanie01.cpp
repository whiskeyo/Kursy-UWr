#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <set>

template <typename T>
std::function<void(T)> compare(const T& a, const T& b)
{
    return [&](const T& c) {
        if (b > c && c > a)
            std::cout << c << " ";
    };
}

template <typename Ts>
auto sum_elements(Ts elements, typename Ts::value_type neutral)
{
    std::for_each(elements.begin(), elements.end(), [&](auto& value) { neutral += value; });
    return neutral;
}

template <typename Ts>
auto find_minmax(const Ts& elements)
{
    auto min = elements.begin(), max = elements.begin(), iter = elements.begin();

    std::for_each(elements.begin(), elements.end(), [&](auto& elem) {
        if (elem < *min)
            min = iter;
        else if (*max < elem)
            max = iter;
        iter++;
    });

    return std::pair{min, max};
}

auto func(std::vector<double> vec_doubles, int p, int k)
{
    std::for_each(vec_doubles.begin() + p, vec_doubles.end(), [&k](double val) {
    static int k_counter = 0;
    if (!k_counter) std::cout << "counter = 0" << std::endl;

    if (k_counter++ % k == 0)
        std::cout << val << " ";
    });
}

int main()
{
    std::vector<double>    vec_doubles({1.21, 3.41, -3.15, 4.312, 4.1235, 7.123, 8.865, 9.12, -1.55, -10.53,
                                        13.4, 512.123, -421.4, -123.5, -1.123, -16.213, 6.77, 6.66, -12.356});
    std::list<std::string> list_strings({"test", "string", "two", "words", "something", "else", "second", "tasty",
                                         "aaa", "dx", "collection", "library", "books", "pomelo", "nasty", "haha",
                                         "exercise", "advanced", "c++", "element", "carbon", "speed", "cache"});
    std::set<int>          set_ints({10, 1, 5, -5, 123, 1621, 1337, -1323, -1235, -62, -612, 512, -215, -5,
                                     11, 125, 521, 2, 745, -8, -546, -456, 2234, -634, -6347, 13521, 532, 56});

    // print all values in range (a, b)
    std::for_each(vec_doubles.begin(), vec_doubles.end(), compare<double>(2.50, 7.150));
    std::cout << std::endl;
    std::for_each(list_strings.begin(), list_strings.end(), compare<std::string>("d", "two"));
    std::cout << std::endl;
    std::for_each(set_ints.begin(), set_ints.end(), compare<int>(500, 1400));
    std::cout << std::endl;

    // print every k-th value starting from p-th value
    constexpr int k = 3, p = 4;

    std::for_each(vec_doubles.begin() + p, vec_doubles.end(), [](double val) {
        static int k_counter = 0;
        if (k_counter++ % k == 0)
            std::cout << val << " ";
    });
    func(vec_doubles, k, p); std::cout << std::endl;
    func(vec_doubles, k, p); std::cout << std::endl;
    func(vec_doubles, k, p); std::cout << std::endl;

    std::cout << std::endl;

    auto list_iterator = list_strings.begin();
    std::advance(list_iterator, p);
    std::for_each(list_iterator, list_strings.end(), [](std::string val) {
        static int k_counter = 0;
        if (k_counter++ % k == 0)
            std::cout << val << " ";
    });
    std::cout << std::endl;

    auto set_iterator = set_ints.begin();
    std::advance(set_iterator, p);
    std::for_each(set_iterator, set_ints.end(), [](int val) {
        static int k_counter = 0;
        if (k_counter++ % k == 0)
            std::cout << val << " ";
    });
    std::cout << std::endl;

    // sum/concatenate all elements
    auto vec_sum  = sum_elements<std::vector<double>>(vec_doubles, 0.0);
    auto list_sum = sum_elements<std::list<std::string>>(list_strings, std::string());
    auto set_sum  = sum_elements<std::set<int>>(set_ints, 0);

    std::cout << "vec_sum: " << vec_sum << std::endl;
    std::cout << "list_concat: " << list_sum << std::endl;
    std::cout << "set_sum: " << set_sum << std::endl;
    
    // calculate average (only vec_doubles, set_ints)
    std::cout << "average (vec): " << vec_sum / static_cast<double>(vec_doubles.size()) << std::endl;
    std::cout << "average (set): " << set_sum / static_cast<double>(set_ints.size()) << std::endl;

    // get min/max iterators
    auto [vec_min, vec_max]   = find_minmax<std::vector<double>>(vec_doubles);
    auto [list_min, list_max] = find_minmax<std::list<std::string>>(list_strings);
    auto [set_min, set_max]   = find_minmax<std::set<int>>(set_ints);

    std::cout << "vec_min: " << *vec_min << ", vec_max: " << *vec_max << std::endl; 
    std::cout << "list_min: " << *list_min << ", list_max: " << *list_max << std::endl; 
    std::cout << "set_min: " << *set_min << ", set_max: " << *set_max << std::endl; 
}