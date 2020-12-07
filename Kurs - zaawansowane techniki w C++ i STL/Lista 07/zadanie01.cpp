#include <iostream>
#include <algorithm>
#include <random>
#include <array>

template <typename T, size_t n>
void shuffle_array (std::array<T, n>& array) {
    static std::mt19937 generator{std::random_device{}()};

    for (size_t i = array.size() - 1; i > 0; --i) {
    // for (size_t i = 1; i < array.size(); i++) {
        size_t j = std::uniform_int_distribution<size_t>{0, i}(generator);
        std::swap(array[i], array[j]);
    }
}

int main() {
    std::array<int, 13> ints = {5, 10, 15, 1, 2, 3, 4, 13, 7, 5, 11, 10, 90};
    std::array<int, 12> ints_sorted = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::array<char, 9> chars = {'b', 'c', 'x', 'd', 'e', 'f', 'g', 'z', 'k'};

    shuffle_array(ints);
    shuffle_array(ints_sorted);
    shuffle_array(chars);

    std::cout << "Shuffled ints: ";
    for (auto& item : ints)
        std::cout << item << " ";

    std::cout << "\nShuffled ints (sorted at first): ";
    for (auto& item : ints_sorted)
        std::cout << item << " ";

    std::cout << "\nShuffled chars: ";
    for (auto& item : chars)
        std::cout << item << " ";    

    std::cout << std::endl;
}