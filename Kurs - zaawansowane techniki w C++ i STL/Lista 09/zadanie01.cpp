#include <iostream>
#include <string>
#include <regex>

// ^ is the beginning of the string, $ is the end of the string
// each group is separated by colon
// 1st group: hour, so three possibilities: 0[0-9], 1[0-9], 2[0-3]
// 2nd group: minutes, match string with [0-5][0-9]
// 3rd group: seconds, match string with [0-5][0-9], but it is optional (? specifier)

bool check_time (const std::string& str_time) {
    std::regex regex_time("^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])(:[0-5][0-9])?$");
    return std::regex_match(str_time, regex_time);
}

int main() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << input << " is " << (check_time(input) ? "correct" : "incorrect") << std::endl;
}