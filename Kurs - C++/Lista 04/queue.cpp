#include "queue.hpp"

Queue::Queue()
{
    first = 0;
    last = 0;
    capacity = 1;
    counter = 0;
    queue = new std::string[1];
}

Queue::Queue(const int k)
{
    first = 0;
    last = 0;  
    capacity = k;
    counter = 0;
    queue = new std::string[k];
}

Queue::Queue(std::initializer_list<std::string> arr)
{
    first = 0;
    last = 0;  
    capacity = arr.size();
    counter = 0;
    queue = new std::string[arr.size()];

    for (auto x : arr)
        enqueue(x);
}

Queue::Queue(const Queue& x)
{
    first = x.first;
    last = x.last;
    capacity = x.capacity;
    counter = 0;
    queue = new std::string[capacity];

    for (int i = 0; i < x.counter; i++)
    {
        queue[i] = x.queue[i];
        counter++;
    }
}

Queue::Queue(Queue&& x)
{
    first = x.first;
    last = x.last;  
    capacity = x.capacity;
    counter = x.counter;
    queue = x.queue;
    x.queue = nullptr;
}

Queue & Queue::operator=(const Queue& x)
{   
    delete [] queue;
    first = x.first;
    last = x.last;
    capacity = x.capacity;
    counter = 0;
    queue = new std::string[capacity];

    for (int i = 0; i < x.counter; i++)
    {
        queue[i] = x.queue[i];
        counter++;
    }

    return *this;
}

Queue & Queue::operator=(Queue&& x)
{
    delete [] queue;

    first = x.first;
    last = x.last;  
    capacity = x.capacity;
    counter = x.counter;
    queue = x.queue;
    x.queue = nullptr;

    return *this;
}

Queue::~Queue()
{
    delete [] queue;
}

bool Queue::isEmpty()
{   
    return (size() == 0);
}

bool Queue::isFull()
{
    return (size() == capacity);
}

void Queue::printQueue()
{
    if (isEmpty())
        throw std::range_error("Queue empty!\n");
    
    for (int i = first; i < last; i++)
        std::cout << queue[i] << std::endl;
}

void Queue::enqueue(const std::string& x)
{
    if (isFull())
        throw std::range_error("Queue full!\n");
    
    queue[last] = x;
    last++;
    counter++;
}

std::string Queue::dequeue()
{
    if (isEmpty())
        throw std::range_error("Queue empty!\n");

    std::string temp = queue[first];

    for (int i = 0; i < last - 1; i++)
        queue[i] = queue[i + 1];

    last--;
    counter--;

    return temp;
}

std::string Queue::peek()
{
    if (isEmpty())
        throw std::range_error("Queue empty!\n");

    return queue[first];
}

int Queue::size()
{
    return counter;
}