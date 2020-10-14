#include <iostream>
#include <cstdint>

// Uzywajac czystej rekurencji dosyc szybko stracimy pamiec do obliczen
auto lucas (uint32_t n)
{
    if (n == 0)
        return 2;
    if (n == 1)
        return 1;
    
    return lucas(n-1) + lucas(n-2);
}

// Lepszym rozwiazaniem jest zastosowanie rekursji ogonowej (z akumulatorem)
// i opakowanie jej w jakis wrapper, aby nie musiec za kazdym razem podawac
// argumentu n1 oraz n2.
auto lucas_tail (uint32_t n, uint64_t n1, uint64_t n2)
{
    if (n == 0)
        return n1;
    if (n == 1)
        return n2;
    return lucas_tail(n - 1, n2, n1 + n2);
}

auto better_lucas (uint32_t n)
{
    return lucas_tail(n, 2, 1);
}

int main()
{
    for (int i = 0; i < 20; i++)
        std::cout << lucas(i) << " ";
    
    std::cout << std::endl;

        for (int i = 0; i < 20; i++)
        std::cout << better_lucas(i) << " ";
    
    std::cout << std::endl;

    return 0;
}