#include <iostream>
#include <string>
#include <exception>
#include <vector>

std::string arabic2roman (int x)
{
    const std::vector<std::pair<int, std::string>> roman =
    {
        {1000, "M"},
        {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
        {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"},
        {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    std::string output = "";
    int result = 0;

    for (int i = 0; i < roman.size(); i++)
    {
        if (result + roman[i].first <= x)
        {
            result += roman[i].first;
            output += roman[i].second;
            i--;
        }
    }

    return output;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        int x;

        try
        {
            x = std::stoi(argv[i]);
        }
        catch (std::logic_error&)
        {
            std::clog << "Invalid argument.\n";
        }

        if (x > 3999 || x < 1)
        {
            std::clog << "Out of range.\n";
            continue;
        }

        std::cout << arabic2roman(x) << std::endl;
    }
}
