#include <iostream>

int main()
{
    std::string str1 = R"(Instytut Informatyki Uniwersytetu Wrocławskiego)";
    std::string str2 = R"(Fryderyka Joliot-Curie 15)";
    std::string str3 = R"(50-383 Wrocław)";
    std::string str4 = R"(C:\Program Files\)";
    std::string str5 = R"x("first test"\n" ()))(") ??>)x";

    std::cout << str1 << std::endl << str2 << std::endl << str3 << std::endl;
    std::cout << str4 << std::endl;
    std::cout << str5 << std::endl;

    return 0;
}