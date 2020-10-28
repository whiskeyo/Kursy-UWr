#include <iostream>
#include <cstdlib>
#include <memory>
#include <random>

class SimpleClass 
{
    private:
        uint64_t counter = 1;

    public:
        ~SimpleClass() { std::cerr << "Value of the counter is: " << counter << std::endl; }
        void add_value(uint64_t val) { counter += val; }

        friend std::ostream& operator << (std::ostream& output, const SimpleClass& sc)
        {
            output << sc.counter;
            return output;
        }
};

bool should_pick_item() // function generates 0 or 1, so we can randomly select items from array later
{
    static std::mt19937 generator{std::random_device()()};
    std::uniform_int_distribution<int> distribution(0, 1);

    return (bool)distribution(generator);
}

void random_addition (std::unique_ptr<SimpleClass[]> array, uint64_t n, uint64_t m, uint64_t i)
{
    if (m == i)
        return;

    for (uint64_t j = 0; j < n; j++)
        if (should_pick_item())
            array[j].add_value(i);

    random_addition(std::move(array), n, m, i + 1);
}

int main()
{
    uint64_t n, m;
    std::cout << "Input values of n, m (separated by space): "; 
    std::cin >> n >> m;

    std::unique_ptr<SimpleClass[]> array{new SimpleClass[n]};
    if (!array) 
    {
        std::cerr << "Allocating memory for an array failed.";
        return 1;
    }

    random_addition(std::move(array), n, m, 0);

    //for (uint64_t i = 0; i < n; i++)
    //    std::cout << array[i] << " ";
}
