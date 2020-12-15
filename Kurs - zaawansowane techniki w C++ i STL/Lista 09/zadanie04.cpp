#include <iostream>
#include <string>
#include <regex>

// match numbers with optional dot and decimal part, separated by optional
// space; added possibility to have plus or minus between real and imaginary

bool check_complex (const std::string& str_complex) {
    std::regex regex_complex(R"l(^\([0-9]*(.[0-9]*)?[ ]?[+-][ ]?[0-9]*(.[0-9]*)?[iI]\)$)l");
    return std::regex_match(str_complex, regex_complex);
}

// I decided to use command line arguments, as it is faster to test programs this way.
int main() {
    std::string complex_expr;
    std::getline(std::cin, complex_expr);

    std::cout << complex_expr << " is " << (check_complex(complex_expr) ? "correct" : "incorrect") << std::endl;
}