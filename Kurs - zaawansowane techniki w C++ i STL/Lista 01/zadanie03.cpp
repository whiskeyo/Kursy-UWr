#include <iostream>
#include <set>

using stringset = std::set<std::string>;

int main()
{
    stringset strings {"string1", "ii", "nice", "el", "stringgggg"};

    for (auto x : strings)
        std::cout << x << std::endl;

    return 0;
}