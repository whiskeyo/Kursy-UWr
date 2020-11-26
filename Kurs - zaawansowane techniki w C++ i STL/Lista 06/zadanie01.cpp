#include <iostream>
#include <cmath>
#include <string>
#include <deque>
#include <random>
#include <algorithm>

class person {
private:
    std::string first_name;
    std::string last_name;
    int age;
    int weight;   // in kgs
    float height; // in meters

public:
    person (std::string first_name, std::string last_name, int age, int weight, float height)
        : first_name(first_name), last_name(last_name), age(age), weight(weight), height(height) {}    

    person (const person& p) {
       first_name = p.first_name;
       last_name  = p.last_name;
       age        = p.age;
       weight     = p.weight;
       height     = p.height;
    }

    // overloaded operators
    bool operator== (const person& p) {
        if (first_name != p.first_name ||
            last_name  != p.last_name  ||
            age        != p.age        ||
            weight     != p.weight     ||
            height     != p.height)
            return false;

        return true;
    }

    // getters and setters
    int   get_age () const         { return age;     }
    void  set_age (int value)      { age = value;    }
    int   get_weight () const      { return weight;  }
    void  set_weight (int value)   { weight = value; }
    float get_height () const      { return height;  }
    void  set_height (float value) { height = value; }

    // other functions
    float calculate_bmi () const { 
        return weight / (height * height); 
    }

    void print_info () const {
        std::cout << first_name << " " << last_name << ", " << age << " years old, " 
                  << height << " meters, " << weight << " kilograms" << std::endl;
    }
};

void print_fighters (const std::string& info, const std::deque<person>& fighters) {
    std::cout << info << std::endl;

    std::for_each(fighters.begin(), fighters.end(), [](const person& fighter){
        fighter.print_info();
    });

    std::cout << std::endl;
}

int main() {
    std::deque<person> fighters = {
        person ("Khabib",        "Nurmagomedov", 32, 70,  1.78),
        person ("Joanna",        "Jedrzejczyk",  33, 52,  1.68),
        person ("Alistair",      "Overeem",      40, 120, 1.93),
        person ("Jennifer",      "Maia",         32, 57,  1.63),
        person ("Conor",         "McGregor",     32, 77,  1.75),
        person ("Nate",          "Diaz",         35, 77,  1.83),
        person ("Khazmat",       "Chimaev",      26, 84,  1.88),
        person ("Floyd",         "Mayweather",   43, 68,  1.73),
        person ("Karolina",      "Kowalkiewicz", 35, 52,  1.60),
        person ("Holly",         "Holm",         39, 61,  1.73),
        person ("Brock",         "Lesnar",       43, 130, 1.91)
    };

    // sort by BMI and print all people
    auto sort_bmi = [](const person& a, const person& b) -> bool { 
        return a.calculate_bmi() < b.calculate_bmi();
    };

    std::sort(fighters.begin(), fighters.end(), sort_bmi);
    print_fighters("Sorted fighters by BMI", fighters);

    // sport trip, set weight to 90%
    auto sport_trip = [&](person& a) {
        a.set_weight(a.get_weight() * 0.9);
    };

    std::for_each(fighters.begin(), fighters.end(), sport_trip);
    print_fighters("Fighters after sport trip", fighters);

    // divide people to two groups: more than 100kg and less than 100kg
    auto more_than_100kg = [](const person& a) { 
        return a.get_weight() > 100; 
    };

    // partition returns iterator
    auto boundary = std::partition(fighters.begin(), fighters.end(), more_than_100kg); 

    std::cout << "Fighters with more than 100kg" << std::endl;
    for (auto iter = fighters.begin(); iter != boundary; ++iter)
        iter->print_info();

    std::cout << "\nFighters with less than 100kg" << std::endl;
    for (auto iter = boundary; iter != fighters.end(); ++iter)
        iter->print_info();

    // find person with median height and change order of people in deque
    auto sort_height = [](const person& a, const person& b) {
        return a.get_height() < b.get_height();
    };

    // std::nth_element instead of sort (is O(n) instead of O(n log n))
    std::cout << "\nPerson with median height: ";
    std::nth_element(fighters.begin(), fighters.begin() + 5, fighters.end(), sort_height);
    fighters[fighters.size() / 2].print_info();

    person median_fighter = person(fighters[fighters.size() / 2]);
    for (unsigned int i = 0; i < fighters.size(); i++) {
        if (fighters[i] == median_fighter) {
            fighters.erase(fighters.begin() + i);
            break;
        }
    }

    fighters.emplace(fighters.begin() + fighters.size() / 2, median_fighter);
    print_fighters("\nChange after median height fighter swap", fighters);

    // random shuffle is depracated in C++14, removed in C++17, though it
    // surprisingly worked on gcc 8.3.0, even with -std=c++2a
    // std::random_shuffle(fighters.begin(), fighters.begin() + fighters.size() / 2);
    // std::random_shuffle(fighters.begin() + fighters.size() / 2 + 1, fighters.end());
    // we have to use std::shuffle (RandomIt first, RandomIt last, URBG&& g) instead:
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(fighters.begin(), fighters.end(), g);
    print_fighters("Random shuffle:", fighters);

    // youngest and oldest person without sorting
    auto compare_age = [](const person& a, const person& b) {
        return a.get_age() < b.get_age();
    };

    auto minmax = std::minmax_element(fighters.begin(), fighters.end(), compare_age);

    std::cout << "Youngest fighter: ";
    fighters[minmax.first - fighters.begin()].print_info();

    std::cout << "Oldest fighter: ";
    fighters[minmax.second - fighters.begin()].print_info();
}