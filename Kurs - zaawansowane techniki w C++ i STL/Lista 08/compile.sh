g++ zadanie01.cpp -o zadanie01.out -std=c++17 -Wall -Wextra -Werror -pedantic;
g++ zadanie02.cpp -o zadanie02.out -std=c++17 -Wall -Wextra -Werror -pedantic;
g++ zadanie02opt.cpp -o zadanie02opt.out -std=c++17 -Wall -Wextra -Werror -pedantic;
g++ zadanie02opt.cpp -o zadanie02optO3.out -std=c++17 -Wall -Wextra -Werror -pedantic -O3;
g++ zadanie02opt.cpp -o zadanie02optOfast.out -std=c++17 -Wall -Wextra -Werror -pedantic -Ofast;

# ./zadanie02.out -30 30 0.01  1000000