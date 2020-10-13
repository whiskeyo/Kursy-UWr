#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <stdint.h>

struct Coin
{
    uint32_t m_nominal, m_mass;
    Coin(uint32_t l_nominal, uint32_t l_mass) : m_nominal(l_nominal), m_mass(l_mass) {}
};

struct Step
{
    uint64_t min, max;
    int8_t temp_min, temp_max;
};

void exerciseResult();

std::vector<Coin> vecofcoins;
uint32_t number_of_coins, total_mass;

int main()
{
    // wylaczenie synchronizacji strumieni
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // wczytanie danych
    std::cin >> total_mass >> number_of_coins;
    vecofcoins.reserve(number_of_coins);
    uint32_t gr_nominal, g_mass;
    for (uint32_t i = 0; i < number_of_coins; i++)
    {
        std::cin >> gr_nominal >> g_mass;
        vecofcoins.push_back(Coin(gr_nominal, g_mass));
    }

    // rozwiazanie zadania
    exerciseResult();
}

void exerciseResult()
{
    Step possibilities[total_mass + 1];

    for (uint32_t i = 0; i <= total_mass; i++)
        possibilities[i] = {0, 0, 0, 0};

    bool possible_mass = true;

    for (uint32_t i = 0; i < total_mass + 1; i++)
        for (uint32_t j = 0; j < number_of_coins; j++)
            if (vecofcoins[j].m_mass <= i)
            {
                if (i != vecofcoins[j].m_mass)
                {
                    possible_mass = false;
                    if (possibilities[i - vecofcoins[j].m_mass].max != 0) possible_mass = true;
                }

                if (possible_mass == false) continue;

                if ((possibilities[i].min > possibilities[i - vecofcoins[j].m_mass].min + vecofcoins[j].m_nominal && (i - vecofcoins[j].m_mass + vecofcoins[j].m_mass == i)) 
                    || (possibilities[i].min == 0 && (i - vecofcoins[j].m_mass + vecofcoins[j].m_mass == i)))
                {
                    possibilities[i].min = possibilities[i - vecofcoins[j].m_mass].min + vecofcoins[j].m_nominal;
                    possibilities[i].temp_min = j;
                }

                if (possibilities[i].max < possibilities[i - vecofcoins[j].m_mass].max + vecofcoins[j].m_nominal 
                    && (i - vecofcoins[j].m_mass + vecofcoins[j].m_mass == i))
                {
                    possibilities[i].max = possibilities[i - vecofcoins[j].m_mass].max + vecofcoins[j].m_nominal;
                    possibilities[i].temp_max = j;
                }
            }

    if (possibilities[total_mass].min == 0)
    {
        std::cout << "NIE" << '\n';
        return;
    }

    std::cout << "TAK" << '\n';

    uint32_t temp_fst = total_mass, temp_sec = total_mass, x;
    uint32_t max[number_of_coins] = {0};
    uint32_t min[number_of_coins] = {0};

    while (temp_fst != 0)
    {
        x = possibilities[temp_fst].temp_min;
        min[x]++;
        temp_fst -= vecofcoins[x].m_mass;
    }

    while (temp_sec != 0)
    {
        x = possibilities[temp_sec].temp_max;
        max[x]++;
        temp_sec -= vecofcoins[x].m_mass;
    }

    std::cout << possibilities[total_mass].min << '\n';

    for (uint32_t i = 0; i < number_of_coins; i++)
        std::cout << min[i] << " ";

    std::cout << '\n' << possibilities[total_mass].max << '\n';

    for (uint32_t i = 0; i < number_of_coins; i++)
        std::cout << max[i] << " ";

    std::cout << '\n';
}