#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility> // std::pair

int main() {
    std::vector<int>   elems = {1, 1, 3, 5, 8, 9, 5, 8, 8, 5};
    std::unordered_map<int, int> ocurrences;

    for (int i = 0; i < (int)elems.size(); i++)
        ocurrences[elems[i]]++;

    std::vector<std::pair<int, int>> frequency_vec(ocurrences.begin(), ocurrences.end());

    std::sort(frequency_vec.begin(), frequency_vec.end(), 
              [](std::pair<int, int> a, std::pair<int, int> b) {
                    // if there is the same number of ocurrences in two numbers
                    // sort them in ascending order
                    if (a.second == b.second)
                        return a.first < b.first;

                    // sort based on frequency in descending order
                    return a.second > b.second;
    });

    // we have sorted vector, so we can easily compare elements with the biggest one
    int max_frequent_elements = 0;
    for (int i = 0; i < (int)frequency_vec.size(); i++) {
        if (frequency_vec[i].second == frequency_vec[0].second)
            max_frequent_elements++;
    }

    for (int i = 0; i < max_frequent_elements; i++)
        std::cout << frequency_vec[i].first << std::endl;
}