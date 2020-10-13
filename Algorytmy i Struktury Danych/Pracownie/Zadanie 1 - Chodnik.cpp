#include <iostream>
#include <map>
#include <utility>
#include <stdint.h>

// mozemy podac dowolny cykl, wiec pamietamy ostatnia wprowadzona dlugosc krawedzi - oszczednosc pamieci
// dane pamietamy w mapie, dzieki czemu nie tworzymy "niepotrzebnych" wierzcholkow, mozemy rowniez uzywac
// operatorow [] do dostepu do odpowiednich danych, tzn. x[lewo][prawo] = waga
using MapOfPairs = std::map<uint16_t, std::map<uint16_t, uint16_t>>;

MapOfPairs m_adjacency_map;
std::string m_result;                       // string przechowujacy wynik (cykl od 0 do 0 z wagami)
bool m_visited_vertices[10001] = {false};   // wierzcholki nie moga sie powtarzac, jako ze mamy jedna dlugosc krawedzi
bool m_good_beginning = false;              // czy cykl konczy sie (i rowniez zaczyna) w 0
uint32_t m_cycle_length = 0;

bool DepthFirstSearch(uint16_t, uint32_t);   // szukanie cyklu, DFS zmienia wartosc wszystkich zmiennych globalnych
std::string makeString(uint16_t, uint16_t, uint16_t); // konwersja danych wierzcholkow do stringa

int main()
{
    // wylaczenie synchronizacji, wykorzystanie buforow (z zasad pracowni)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // input
    uint32_t n;
    uint16_t left, middle, right;

    std::cin >> n;
    for (uint32_t i = 0; i < n; i++)
    {
        std::cin >> left >> middle >> right;
        if (left == 0 && right == 0) // krotki chodnik, tj. petla w wierzcholku 0
        {
            std::cout << "1\n" << left << " " << middle << " " << right << "\n";
            return 0;
        }
        // dodanie wartosci tak jak zapisane w 1. komentarzu, nie musimy pamietac wszystkich
        // wag pomiedzy krawedziami, jako ze szukamy dowolnego cyklu
        m_adjacency_map[left][right] = middle;
    }

    // rozwiazanie zadania
    DepthFirstSearch(0, 0);
    if (m_good_beginning == true)
        std::cout << m_cycle_length << "\n" << m_result;
    else
        std::cout << "BRAK";
    
    return 0;
}

bool DepthFirstSearch(uint16_t l_vertex, uint32_t l_counter)
{
    m_visited_vertices[l_vertex] = true;
    
    for (const auto &iter : m_adjacency_map[l_vertex])
    {
        // sprawdzenie czy cykl dobrze sie zaczyna/konczy + manualne dodanie ostatniej krawedzi
        if (iter.first == 0)
        {
            m_good_beginning = true;
            m_cycle_length = 1 + l_counter;
            m_result = makeString(l_vertex, m_adjacency_map[l_vertex][0], 0) + m_result;
            
            return true;
        }
        // jak nie jest ostatnim elementem i nie byl odwiedzony, to sprawdzaj sasiadow
        else if (m_visited_vertices[iter.first] == false && DepthFirstSearch(iter.first, l_counter + 1) == true)
        {
            m_result = makeString(l_vertex, iter.second, iter.first) + m_result;
            return true;
        }
    }

    return false;
}

std::string makeString(uint16_t l_left, uint16_t l_middle, uint16_t l_right)
{
    return std::to_string(l_left) + " " + std::to_string(l_middle) + " " + std::to_string(l_right) + "\n";
}