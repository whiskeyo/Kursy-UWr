#include <iostream>
#include <cstdlib>
#include <random>
#include <chrono>

template <uint64_t N>
class Matrix
{
    private:
        double* array;

        static double generateNumber()
        {
            static std::mt19937 generator{std::random_device()()};
            return std::uniform_real_distribution<double>(0.5, 2.0)(generator);
        }
    
    public:
        Matrix() : array(new double[N * N])
        {
            for (uint64_t i = 0; i < N; i++)
                for (uint64_t j = 0; j < N; j++)
                    array[i * N + j] = generateNumber();
        }

        ~Matrix() { delete[] array; }

        Matrix(const Matrix&) = delete;
        Matrix& operator=(const Matrix&) = delete;
        
        Matrix(Matrix&& matrix) : array(matrix->array)
        {
            matrix->array = nullptr;
        }

        Matrix& operator=(Matrix&& matrix) 
        {
            if (this != matrix)
                std::swap(array, matrix->array);
            return matrix;
        }

        
        Matrix& square()
        {
            auto result = new double[N * N];

            for (uint64_t i = 0; i < N; i++)
                for (uint64_t j = 0; j < N; j++)
                {
                    result[i * N + j] = 0.0;
                    for (uint64_t k = 0; k < N; k++)
                        result[i * N + j] += array[i * N + k] * array[k * N + j];
                }

            delete[] array;
            array = result;
            return *this;
        }
};

template <uint64_t N>
void measureTime(Matrix<N>& matrix, uint32_t iterations)
{
    using namespace std::chrono;
    auto startTime = high_resolution_clock::now();

    for (uint32_t i = 0; i < iterations; i++)
        matrix.square();

    auto endTime = high_resolution_clock::now();
    auto time = duration_cast<duration<double>>( (endTime - startTime) / iterations );

    std::cout << N << "x" << N << " matrix multiplication took " << time.count() << " seconds." << std::endl;
}

int main()
{
    Matrix<10>    matrix10;
    Matrix<100>   matrix100;
    Matrix<1000>  matrix1000;
    //Matrix<10000> matrix10000;

    measureTime(matrix10, 1000);
    measureTime(matrix100, 100);
    measureTime(matrix1000, 10);
    //measureTime(matrix10000, 1);
}

/* Results with default compiling settings:
 * 10x10: 4.522e-07 seconds
 * 100x100: 0.00507409 seconds
 * 1000x1000: 5.81949 seconds
 * 10000x10000: I gave up after 10 minutes
 * 
 * Results with -O3 flag:
 * 10x10: 2.5e-07 seconds
 * 100x100: 0.00111421 seconds
 * 1000x1000: 2.17892 seconds
 * 10000x10000: I gave up after 10 minutes
 * 
 * In both cases for 10000x10000 matrix it would
 * take approximately 1000 times longer than 
 * for 1000x1000 matrix.
 */
