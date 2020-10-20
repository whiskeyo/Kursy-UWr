#include <iostream>
#include <cstdint>

enum class name : uint16_t 
{
    Tomasz, Bartosz, Arkadiusz, Dawid, Konrad, Mikolaj
};

void func (const std::string& str, name person)
{
    const auto convert_name = [](name person) -> std::string {
        switch (person)
        {
            case name::Tomasz:    return "Tomasz";
            case name::Bartosz:   return "Bartosz";
            case name::Arkadiusz: return "Arkadiusz";
            case name::Dawid:     return "Dawid";
            case name::Konrad:    return "Konrad";
            case name::Mikolaj:   return "Mikolaj";
        }

        return "";
    };
    
    std::cout << convert_name(person) << ": " << str << std::endl;
}

int main()
{
    func("Jakis przykladowy tekst", name::Dawid);
    return 0;
}