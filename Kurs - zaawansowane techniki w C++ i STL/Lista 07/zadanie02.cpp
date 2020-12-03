#include <iostream>
#include <fstream>
#include <random>

int main() {
    std::mt19937 generator {std::random_device{}()};
    constexpr int n_elems = 10000;

    std::ofstream uniform("uniform.csv");
    std::ofstream binomial("binomial.csv");
    std::ofstream normal("normal.csv");

    auto uniform_distro  = std::uniform_real_distribution<double>{0, 1000};
    auto binomial_distro = std::binomial_distribution<int>{1000, 0.5};
    auto normal_distro   = std::normal_distribution<double>{500, 50};

    for (int i = 0; i < n_elems; i++) {
        uniform << uniform_distro(generator) << std::endl;
        binomial << binomial_distro(generator) << std::endl;
        normal << normal_distro(generator) << std::endl;
    }

    // system("gnuplot -e \"set terminal png size 500,500; set output 'uniform.png'; plot 'uniform.csv'\"");
    // system("gnuplot -e \"set terminal png size 500,500; set output 'binomial.png'; plot 'binomial.csv'\"");
    // system("gnuplot -e \"set terminal png size 500,500; set output 'normal.png'; plot 'normal.csv'\"");

    system("gnuplot distributions.gp");
}