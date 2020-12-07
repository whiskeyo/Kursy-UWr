#include <iostream>
#include <complex>

constexpr double euler_gamma = 0.5772156649;

std::complex<double> gamma (std::complex<double> z, int iterations) {
    std::complex<double> result = 1.0 / z;

    for (int n = 1; n <= iterations; n++) 
        result *= std::pow(1.0 + 1.0 / static_cast<double>(n), z) / (1.0 + z / static_cast<double>(n));
    
    return result;
}

std::complex<double> inverse_gamma (std::complex<double> z, int iterations) {
    std::complex<double> result = z * std::exp(euler_gamma * z);

    for (int n = 1; n <= iterations; n++)
        result *= (1.0 + z / static_cast<double>(n)) * std::exp(z / static_cast<double>(n));

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Incorrect input! Usage: " << argv[0] << " <real> <imaginary> <iterations>" << std::endl;
        return -1;
    }

    double real      = std::stod(argv[1]);
    double imaginary = std::stod(argv[2]);
    int iterations   = std::stoi(argv[3]);

    std::complex<double> z(real, imaginary);

    std::cout << "Γ(z)   = " << gamma(z, iterations) << std::endl;
    std::cout << "1/Γ(z) = " << inverse_gamma(z, iterations) << std::endl;
}
