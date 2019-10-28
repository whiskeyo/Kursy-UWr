public class Consumer extends Thread
{
    Buffer<String> buffer;
    int maxElems;
    int countSaved;

    public Consumer(Buffer<String> buffer)
    {
        this.maxElems = buffer.maxCapacity;
        this.buffer = buffer;
        countSaved = 0;
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                System.out.println("Element deleted: " + buffer.save());
                countSaved++;
            }
            catch (InterruptedException e)
            {
                // without this case it can not compile, as this exception checks whether
                // wait() was used or not
            }
            if (this.countSaved == this.maxElems)
                return;
        }
    }
}