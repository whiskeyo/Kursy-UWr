#include <iostream>
#include <string>
#include <regex>

bool check_date (const std::string& str_date) {
    std::regex regex_date(
        // exclude the possibility of day 00 and year 0000
        "^((?!00)([012][0-9]|30|31)-(01|03|05|07|08|10|12)-(?!0000)[0-9]{4})" // capture all 31 days months
        "|((?!00)([012][0-9]|30)-(04|06|09|11)-(?!0000)[0-9]{4})" // capture all 30 days months
        "|((?!00)([01][0-9]|2[0-8])-02-(?!0000)[0-9]{4})" // capture all 28 days Februaries
        // capture all 29 days Februaries - leap years (ending with XXYY, where YY is multiple of 4)
        "|((?!00)[012][0-9]-02-(?!0000)[0-9]{2}(00|04|08|12|16|20|24|28|32|36|40|44|48|52|56|60|64|68|72|76|80|84|88|92|96))$"
    );
    return std::regex_match(str_date, regex_date);
}

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << input << " is " << (check_date(input) ? "correct" : "incorrect") << std::endl;
}