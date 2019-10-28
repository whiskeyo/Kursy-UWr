public class Producer extends Thread
{
    Buffer<String> buffer;
    int maxElems;
    int countProduced;

    public Producer(Buffer<String> buffer)
    {
        this.maxElems = buffer.maxCapacity;
        this.buffer = buffer;
        countProduced = 0;
    }

    @Override
    public void run()
    {
        while (true)
        {
            try
            {
                buffer.insert("Produced string number: " + String.valueOf(countProduced));
                countProduced++;
            }
            catch (InterruptedException e)
            {
                // without this case it can not compile, as this exception checks whether
                // wait() was used or not
            }
            if (this.countProduced == this.maxElems)
                return;
        }
    }
}