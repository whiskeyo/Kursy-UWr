#include <iostream>

typedef struct date
{
    int day, month, year;
} date;

date parse_date(std::string input)
{
    int date_fields[3] = {}, i = 0;
    size_t pos = 0;
    std::string delimiter = "-";
    while ((pos = input.find(delimiter)) != std::string::npos)
    {
        date_fields[i] = std::stoi(input.substr(0, pos));
        input.erase(0, pos + delimiter.length());
        i++;
    }
    date_fields[2] = std::stoi(input); // po drugim delimiterze konczy sie petla

    return date{date_fields[0], date_fields[1], date_fields[2]};
}

int main()
{
    std::string input;
    std::cout << "Podaj datę w formacie dd-mm-rrrr: "; 
    std::cin >> input;

    std::string month;
    switch (auto i = parse_date(input).month; i)
    {
        case 1:  month = "stycznia";     break;
        case 2:  month = "lutego";       break;
        case 3:  month = "marca";        break;
        case 4:  month = "kwietnia";     break;
        case 5:  month = "maja";         break;
        case 6:  month = "czerwca";      break;
        case 7:  month = "lipca";        break;
        case 8:  month = "sierpnia";     break;
        case 9:  month = "wrzesnia";     break;
        case 10: month = "pazdziernika"; break;
        case 11: month = "listopada";    break;
        case 12: month = "grudnia";      break;
    }

    std::cout << "Podana data: " << parse_date(input).day << " " << month << " " << parse_date(input).year << " roku." << std::endl;
    
    return 0;
}
