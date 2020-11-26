#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect input! Usage: " << argv[0] << "<filename>" << std::endl;
        return -1;
    }

    // read content of the file
    std::ifstream input_stream(argv[1]);
    std::string   file_content;
    getline(input_stream, file_content, (char)input_stream.eof());

    // remove whitespaces and special characters, and convert all letters to lowercase
    file_content.erase(
        std::remove_if(file_content.begin(), 
                       file_content.end(), 
                       [](char c) { return !std::isalpha(c); }), 
        file_content.end());

    std::transform(file_content.begin(), file_content.end(), file_content.begin(), [](char c) {
        return std::tolower(c);
    });

    // calculate all occurrences of letters
    size_t letters_in_file = file_content.size();
    std::map<char, int> frequencies;

    for (auto& letter : file_content)
        frequencies[letter]++;

    std::cout << "Number of occurences of letters and their frequency in text" << std::endl;
    for (auto& elem : frequencies)
        std::cout << elem.first << ": " << elem.second << ", " << (float)elem.second / letters_in_file * 100 << "%" << std::endl;
}