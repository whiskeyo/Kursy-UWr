#include <iostream>
#include <fstream>
#include <complex>

constexpr double euler_gamma = 0.5772156649;

std::complex<double> gamma (std::complex<double> z, int iterations) {
    std::complex<double> result = 1.0 / z;

    for (int n = 1; n <= iterations; n++) 
        result *= std::pow(1.0 + 1.0 / static_cast<double>(n), z) 
                / (1.0 + z / static_cast<double>(n));
    
    return result;
}

std::complex<double> gamma_opt (std::complex<double> z, int iterations) {
    std::complex<double> result = 1.0 / z;

    for (int n = 1; n <= iterations; n++) {
        double inv_n  = 1.0 / static_cast<double>(n);
        std::complex<double> z_by_n = z / static_cast<double>(n);
        result *= std::pow(1.0 + inv_n, z) / (inv_n + z_by_n);
    }
    
    return result;
}

// From en.cppreference.com/w/cpp/numeric/special_functions/riemann_zeta there
// are few cases for Riemann zeta function depending on the value of real part.
std::complex<double> zeta (std::complex<double> z, int iterations) {
    std::complex<double> result = 0.0;

    if (z.real() > 1) {
        for (int n = 1; n <= iterations; n++) 
            result += 1.0 / std::pow(static_cast<double>(n), z);
    
        return result;
    } else if (z.real() >= 0 && z.real() <= 1) {
        for (int n = 1; n <= iterations; n++)
            result += std::pow(-1.0, n + 1) * std::pow(n, -z);

        return result / (1.0 - std::pow(2.0, -z + 1.0));
    } else { // z.real() < 0
        for (int n = 1; n <= iterations; n++)
            result = std::pow(2.0, z) * std::pow(M_PI, z - 1.0) 
                   * sin(M_PI * z / 2.0) * gamma_opt(1.0 - z, iterations) 
                   * zeta(1.0 - z, iterations);
                 
        return result;           
    }
}

std::complex<double> zeta_convergent (std::complex<double> z, int iterations) {
    std::complex<double> result = 0.0;

    for (int n = 1; n <= iterations; n++)
        result += (n % 2 ? 1.0 : -1.0) * std::pow(n, -z);

    return result / (1.0 - std::pow(2.0, -z + 1.0));
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Incorrect input!" << std::endl; 
        std::cerr << "Usage: " << argv[0] << " <min> <max> <step> <iterations>" << std::endl;
        return -1;
    }

    double min     = std::stod(argv[1]);
    double max     = std::stod(argv[2]);
    double step    = std::stod(argv[3]);
    int iterations = std::stoi(argv[4]);
    std::ofstream output("results_opt.csv");

    auto critical_line = [&](double imaginary) {
        return zeta_convergent({0.5, imaginary}, iterations);
    };

    for (double value = min; value <= max; value += step) {
        auto result = critical_line(value);
        output << value << "; " << result.real() << "; " << result.imag() << std::endl;
    }
    
    // Comparing the plot with the one on Wikipedia, the function is working well!
    system("gnuplot critical_line_opt.gp");
}
