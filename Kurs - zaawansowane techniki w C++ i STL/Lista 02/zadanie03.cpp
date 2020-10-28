#include <iostream>
#include <memory>

int main() 
{
    std::weak_ptr<char> weak_char;

    // Ograniczam scope do dzialajacego weak_ptr
    {
        auto shared = std::make_shared<char>('x');
        weak_char = shared;
        std::cout << *weak_char.lock() << std::endl;
    }

    // A to wywali segfault, bo shared juz nie istnieje
    // wiec odwolujemy sie do pamieci spoza programu.
    
    if (weak_char.expired())
        std::cout << "weak_char juz nie istnieje" << std::endl;
    else
        std::cout << "weak_char dalej istnieje" << std::endl;
    

    //std::cout << *weak_char.lock() << std::endl;

    return 0;
}
