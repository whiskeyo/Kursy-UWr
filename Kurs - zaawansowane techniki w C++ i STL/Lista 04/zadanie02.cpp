#include <iostream>
#include <functional>

// W tresci zadania jest do zdefiniowania adapter compose_f_gx_hy, jednak
// na przykladzie jest napisane o funkcji f(g(x), h(x)). Zakladam, ze 
// przyklad jest bledny i nalezy zaimplementowac adapter do zlozenia
// funkcji f(g(x), h(y)), tak jak mowi jego nazwa, choc rozwiazanie nie
// bedzie sie praktycznie wcale roznic oprocz tego, ze przekazywane beda
// dwa argumenty do funkcji zamiast jednego.

template <typename F, typename G, typename H>
class Compose_f_gx_hy
{
private:
    F _f;
    G _g;
    H _h;

public:
    Compose_f_gx_hy(F f, G g, H h) : _f(f), _g(g), _h(h) {};

    typename F::result_type operator() (typename G::argument_type x, 
                                        typename H::argument_type y) 
    {
        return _f(_g(x), _h(y));
    }
};

template <typename F, typename G, typename H>
Compose_f_gx_hy<F, G, H> compose_f_gx_hy(F f, G g, H h) 
{
    return Compose_f_gx_hy<F, G, H>(f, g, h);
}

int square(int x)     { return x * x; }
int increment(int x)  { return x + 1; }
int decrement(int x)  { return x - 1; }
int add(int x, int y) { return x + y; }

int main()
{
    // TODO: ogarnac jak dziala skladanie funkcji w tak zaimplementowanym
    // adapterze, bo poki co wywala pelno bledow :(
}