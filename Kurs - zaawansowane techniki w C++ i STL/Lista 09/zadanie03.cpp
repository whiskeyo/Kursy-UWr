#include <iostream>
#include <string>
#include <regex>

bool check_city (const std::string& str_city) {
    std::regex regex_date(
        // first part can occur 0-infty times (word starting with upper letter)
        "^(([A-Z]|Ć|Ł|Ń|Ó|Ś|Ź|Ż)([a-z]|ą|ć|ę|ł|ń|ó|ś|ź|ż){1,}[ ]{1,}){0,}"
        // hyphenated part, it is optional, thus "?" is at the end
        "([A-Z]|Ć|Ł|Ń|Ó|Ś|Ź|Ż)([a-z]|ą|ć|ę|ł|ń|ó|ś|ź|ż){1,}(-([A-Z]|Ć|Ł|Ń|Ó|Ś|Ź|Ż)([a-z]|ą|ć|ę|ł|ń|ó|ś|ź|ż){1,})?"
        // last part can occur 0-infty times, similar to first part, but space is at the beginning
        "([ ]{1,}([A-Z]|Ć|Ł|Ń|Ó|Ś|Ź|Ż)([a-z]|ą|ć|ę|ł|ń|ó|ś|ź|ż){1,}){0,}$"
    );
    return std::regex_match(str_city, regex_date);
}

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << input << " is " << (check_city(input) ? "correct" : "incorrect") << std::endl;
}