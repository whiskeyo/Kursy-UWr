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
    // this part made the code work in O(n^2) time, but it can be solved in O(n)
    // file_content.erase(
    //     std::remove_if(file_content.begin(), 
    //                    file_content.end(), 
    //                    [](char c) { return !std::isalpha(c); }), 
    //     file_content.end());

    // std::transform(file_content.begin(), file_content.end(), file_content.begin(), [](char c) {
    //     return std::tolower(c);
    // });

    // calculate all occurrences of letters
    size_t letters_in_file = 0;
    int frequencies[26] = {};

    for (auto& letter : file_content) {
        letters_in_file++;
        
        if (letter >= 'a' && letter <= 'z')
            frequencies[(int)letter - 97]++;
        else if (letter >= 'A' && letter <= 'Z')
            frequencies[(int)letter - 65]++;
    }

    std::cout << "Number of occurences of letters and their frequency in text" << std::endl;
    for (int i = 0; i < 26; i++)
        std::cout << (char)(i + 97) << ": " << frequencies[i]<< ", " << (double)frequencies[i] / letters_in_file * 100 << "%" << std::endl;
}