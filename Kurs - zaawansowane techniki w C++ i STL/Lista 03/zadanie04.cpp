#include <iostream>
#include <type_traits>
#include <string>

// Wersja zadana przez referencje, jak da sie skonwertowac
// z From do To, to przenosimy wartość.
template <typename From, typename To>
void moveFromTo(From& from, To& to, std::false_type)
{
    if constexpr (std::is_convertible<From, To>::value)
    {
        to = static_cast<To&&>(std::move(from));
        
        //to = (To&&)(std::move(from));
        std::cout << "Used no pointer version." << std::endl;
    }
    else  
        std::cout << "Failed to convert." << std::endl;
}

// Wersja ze wskaznikiem, najpierw sprawdzamy czy da sie
// skonwertowac From* (po usunieciu wskaznika) do To, pozniej
// dzieje sie to samo co wyzej (tylko bierzemy *from).
template <typename From, typename To>
void moveFromTo(From from, To& to, std::true_type)
{
    if constexpr (std::is_convertible<typename std::remove_pointer<From>::type, To>::value)
    {
        to = (To&&)(std::move(*from));
        std::cout << "Used pointer version." << std::endl;
    }
    else  
        std::cout << "Failed to convert." << std::endl;
}

// Wrapper do obu funkcji
template <typename From, typename To>
void moveFromTo(From from, To& to)
{
    moveFromTo(from, to, std::is_pointer<From>());
}



int main()
{
    // Sprawdzenie dla wersji bez wskaznikow:
    std::string str1 = "first string";
    std::string str2 = "second string";

    moveFromTo(str1, str2);
    std::cout << str2 << std::endl;

    // Sprawdzenie dla wersji ze wskaznikiem:
    std::string str3  = "third string";
    std::string *str4 = new std::string("fourth string (pointer)");

    moveFromTo(str4, str3);
    std::cout << str3 << std::endl;

    // Sprawdzenie dla dwoch roznych typow:
    struct FirstStruct {};
    struct SecondStruct { FirstStruct first; operator FirstStruct() { return first; }};

    FirstStruct a;
    SecondStruct b;

    moveFromTo(b, a); 
}