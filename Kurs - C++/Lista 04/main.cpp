#include <iostream>
#include "queue.hpp"

int main()
{
    int switcher = 0;
    std::string temp;
    Queue my_queue(5);

    system("clear");

    while (switcher != 5)
    {
        std::cout << "What do you want to do with a queue?" << std::endl;
        std::cout << "1. Enqueue an element (add to the queue)." << std::endl;
        std::cout << "2. Dequeue an element (remove from the queue)." << std::endl;
        std::cout << "3. Print first element of the queue." << std::endl;
        std::cout << "4. Print the queue." << std::endl;
        std::cout << "5. Quit." << std::endl;
        std::cin >> switcher;
        std::getchar();

        switch (switcher)
        {
            case 1:
                system("clear");
                std::cout << "Insert the string you want to add: ";
                std::getline(std::cin, temp);
                my_queue.enqueue(temp);
                break;
            case 2:
                system("clear");
                std::cout << "Element " << my_queue.dequeue() << " dequeued." << std::endl;
                break;
            case 3:
                system("clear");
                std::cout << "Element " << my_queue.peek() << " is at the peek." << std::endl;
                break;
            case 4:
                system("clear");
                std::cout << "Your queue: " << std::endl;
                my_queue.printQueue();
                break;
            case 5:
                system("clear");
                std::cout << "Quitting." << std::endl;
                break;
            default:
                system("clear");
                std::cout << "Wrong input (pick option 1-5)." << std::endl;
                break;                
        }
    }

    // Test of constructors
    Queue a({"aaa", "bbb", "ccc", "ddd"});
    Queue b({"b", "bb", "bbb", "bbbb"});
    Queue c({"c", "cc", "ccc", "cccc"});
    a.printQueue();
    b.printQueue();
    c.printQueue();
    
    c = b;                      // copy b Queue into c, b stays unchanged
    c.printQueue();
    Queue e = std::move(a);     // move a Queue to e (so a is removed)
    e.printQueue();
}