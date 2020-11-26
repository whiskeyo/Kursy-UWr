#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility> // std::pair

int main() {
    std::vector<int>   elems = {1, 1, 3, 5, 8, 9, 5, 8, 8, 5, 9, 9};
    std::unordered_map<int, int> ocurrences;

    for (unsigned int i = 0; i < elems.size(); i++)
        ocurrences[elems[i]]++;

    std::vector<std::pair<int, int>> frequency_vec(ocurrences.begin(), ocurrences.end());

    auto compare_freq = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second < b.second;
    };

    auto most_occurrences = std::max_element(frequency_vec.begin(), frequency_vec.end(), compare_freq)->second;
    for (unsigned int i = 0; i < frequency_vec.size(); i++) 
        if (frequency_vec[i].second == most_occurrences)
            std::cout << frequency_vec[i].first << std::endl;
}