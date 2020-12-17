#include <iostream>
#include <string>
#include <regex>

bool check_city (const std::string& str_city) {
    const std::string upper = "[A-Z]|Ć|Ł|Ń|Ó|Ś|Ź|Ż";
    const std::string lower = "[a-z]|ą|ć|ę|ł|ń|ó|ś|ź|ż";
    
    const std::string regex_pattern(
        // first part can occur 0-infty times (word starting with upper letter)
        "^((" + upper + ")(" + lower + "){1,}[ ]{1,}){0,}"
        // hyphenated part, it is optional, thus "?" is at the end
        "(" + upper + ")(" + lower + "){1,}(-(" + upper + ")(" + lower + "){1,})?"
        // last part can occur 0-infty times, similar to first part, but space is at the beginning
        "([ ]{1,}(" + upper + ")(" + lower + "){1,}){0,}$"
    );

    std::regex regex_city(regex_pattern);

    return std::regex_match(str_city, regex_city);
}

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << input << " is " << (check_city(input) ? "correct" : "incorrect") << std::endl;
}