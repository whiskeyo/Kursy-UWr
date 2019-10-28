public class MergeSort implements Runnable
{
    int[] array;
    int[] temp;
    int begin;
    int end;

    MergeSort(int[] array, int begin, int end)
    {
        this.array = array;
        this.begin = begin;
        this.end   = end;

        temp = new int [end + 1];
    }

    private void Merge()
    {
        int i, j, k;
        int avg = (begin + end) / 2;

        for (i = begin; i <= end; i++)
            temp[i] = array[i];

        i = begin;
        j = avg + 1;
        k = begin;

        while (i <= avg && j <= end)
        {
            if (temp[i] < temp[j])
                array[k++] = temp[i++];
            else
                array[k++] = temp[j++];
        }

        while (i <= avg)
            array[k++] = temp[i++];
        
        while (j <= end)
            array[k++] = temp[j++];
    }

    public void run()
    {
        if (begin < end)
        {
            int avg = (begin + end) / 2;

            MergeSort sort_left  = new MergeSort(array, begin, avg);
            MergeSort sort_right = new MergeSort(array, avg + 1, end);

            Thread thread_left  = new Thread(sort_left);
            Thread thread_right = new Thread(sort_right);

            thread_left.start();
            thread_right.start();

            // We need to use try-catch, as we need to work with join() method. Thanks to it, thread working at the moment
            // will wait until its work is finished. That way we may be sure that arrays are not merged before sorting is done.

            try {thread_left.join();}
            catch (InterruptedException e) {e.printStackTrace();}

            try {thread_right.join();}
            catch (InterruptedException e) {e.printStackTrace();}
            
            Merge();
        }
    }
}