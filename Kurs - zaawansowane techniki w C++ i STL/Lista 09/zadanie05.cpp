#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

void print_tags (const std::vector<std::string>& doc) {
    // match <a> tag with href and any optional additional specifiers
    // closing tab can be <a /> or </a>, thus it picks one of these options
    std::regex regex_tag(R"l(<a.*?href="(.*?)".*?>.*?<\/a>)l");
    for (auto line : doc) {
        std::smatch match;
        while (std::regex_search(line, match, regex_tag)) {
            std::cout << match.str(1) << std::endl;
            line = match.suffix().str();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect input! Usage: " << argv[0] << " <filename>" << std::endl;
        return -1;
    }

    std::ifstream input_file(argv[1]);
    std::string line;
    std::vector<std::string> doc;

    while (std::getline(input_file, line)) {
        if (line.empty())
            continue;
        doc.push_back(line);
    }

    print_tags(doc);
}