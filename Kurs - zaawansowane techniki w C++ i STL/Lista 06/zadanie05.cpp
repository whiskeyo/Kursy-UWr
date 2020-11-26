#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect input! Usage: " << argv[0] << "<string>" << std::endl;
        return -1;
    }

    std::string str(argv[1]);

    // We need to sort the string, as std::next_permutation
    // permutes string in that way, that it is contains the
    // unique permutation that would appear in lexicographical
    // order. If we did not sort the string, we would not get
    // all (length)! permutations.

    std::sort(str.begin(), str.end());

    do {
        std::cout << str << std::endl;
    } while (std::next_permutation(str.begin(), str.end()));
}