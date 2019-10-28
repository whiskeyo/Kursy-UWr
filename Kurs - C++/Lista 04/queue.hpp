#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

class Queue
{
    private:
        int first;
        int last;
        int capacity;
        int counter;
        std::string* queue = nullptr;
    
    public:
        Queue();
        Queue(int);
        Queue(std::initializer_list<std::string>);
        Queue(Queue&&);
        Queue(const Queue&);

        ~Queue();

        Queue & operator=(const Queue&);
        Queue & operator=(Queue&&);

        bool isEmpty();
        bool isFull();
        void printQueue();

        void enqueue(const std::string&);
        std::string dequeue();
        std::string peek();
        int size();
};

#endif // QUEUE_HPP