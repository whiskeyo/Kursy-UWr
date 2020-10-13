#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <cmath>

int magicznepiatki(std::vector<int> vec, int n, int k) {
    // dla malej ilosci elementow (<20) sortujemy tablice i bierzemy k-ty element
    if (n < 20) {
        std::sort(vec.begin(), vec.end());
        return vec[k-1]; // zwracamy k-1, dlatego ze indeksy mamy od 0
    } else {
        // w przeciwnym przypadku dzielimy tablice na podciagi 5-elementowe
        // dla n nie bedacego dzielnikiem 5 ostatni podciag uzupelniamy +inf
        // (czyli wartosc spoza zakresu danych), n_temp to ilosc blokow po 5
        int n_temp = (n + 4) / 5;
        std::vector<int> P;
        P.reserve(n);

        // dane z tablicy wrzucamy do wektora
        for (int i = 0; i < n; i++)
            P.push_back(vec[i]);
        
        // dla n nie bedacego dzielnikiem 5 ostatni podciag uzupelniamy wartosciami nieosiagalnymi (z danych)
        int psize = P.size();
        if (psize % 5 != 0) {
            int i = psize % 5;
            while (i < 5) {
                P.push_back(1000000001); // tu rownie dobrze mozna wrzucic INT_MAX, nie zmieni to wyniku
                i++;
            }
        }

        // sortujemy kazda piatke 
        for (int i = 0; i < n_temp; i++)
            std::sort(P.begin() + i * 5, P.begin() + i * 5 + 5);

        // szukamy median w podciagach (jako ze sa posortowane, to bierzemy 3. element)
        std::vector<int> M;
        for (unsigned int i = 0; i < (unsigned)P.size() - 4; i = i + 5)
            M.push_back(P[i + 2]);

        // i wywolujemy sie rekurencyjnie
        int m = magicznepiatki(M, M.size(), (int)floor(M.size() / 2));

        // dzielimy elementy na trzy grupy: mniejsze od m, rowne m, wieksze od m
        std::vector<int> mniejsze, rowne, wieksze;
        for (unsigned int i = 0; i < P.size(); i++) {
            if (P[i] < m)  
                mniejsze.push_back(P[i]);
            else if (P[i] == m)  
                rowne.push_back(P[i]);
            else
                wieksze.push_back(P[i]);
        }

        if ((unsigned)k <= mniejsze.size())
            return magicznepiatki(mniejsze, mniejsze.size(), k);
        else if ((unsigned)k <= mniejsze.size() + rowne.size())
            return m;
        else
            return magicznepiatki(wieksze, wieksze.size(), k - mniejsze.size() - rowne.size());
    }
}

int main() {
    int n, k, temp;
    std::vector<int> v;

    std::cin >> n >> k;
    v.reserve(n);
    for (int i = 0; i < n; i++) {
        std::cin >> temp;
        v.push_back(temp);
    }

    int kth = magicznepiatki(v, n, k);
    std::cout << kth << std::endl;
}