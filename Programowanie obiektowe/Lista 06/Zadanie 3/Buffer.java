import java.util.ArrayList;

public class Buffer<T>
{
    ArrayList<T> list;
    public int maxCapacity;
    int countElems;

    public Buffer(int maxCapacity)
    {
        this.maxCapacity = maxCapacity;
        this.list = new ArrayList<T>(maxCapacity);
        countElems = 0;
    }

    public synchronized void insert (T elem) throws InterruptedException
    {
        while (this.isFull() == true) // when the buffer is full it waits until some space is released
            wait();
        
        list.add(elem);
        countElems++;
        notify();
        return;
    }

    public synchronized T save () throws InterruptedException
    {
        while (this.isEmpty() == true) // when the buffer is empty it waits until something is added
            wait();

        countElems--;

        notify();
        return list.remove(0);
    }

    public boolean isFull()
    {
        if (countElems == maxCapacity)
            return true;
        else
            return false;
    }

    public boolean isEmpty()
    {
        if (countElems == 0)
            return true;
        else
            return false;
    }
}