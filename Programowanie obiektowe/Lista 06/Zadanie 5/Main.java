// Algorytm sortowania tablicy elementów przez scalanie działa następująco: najpierw tablica jest dzielona na pół.
// Następnie każda z tych mniejszych tablic jest porządkowana. Na końcu obydwie posortowane tablice są scalane.
// Zaprogramuj sortowanie przez scalanie tablic elementów int tak, aby operacje sortowania podtablic były osobnymi wątkami.

public class Main
{
    public static void main(String[] args)
    {
        int[] arr = {9, 4, 12, 61, 77, 51, 92, -3, 67};

        MergeSort sort = new MergeSort(arr, 0, arr.length - 1);
        Thread thread  = new Thread(sort);

        try {thread.start(); thread.join();}
        catch (InterruptedException e) {e.printStackTrace();}
        
        System.out.println("Sorted array: ");
        for (int i = 0; i < arr.length; i++)
            System.out.println(arr[i]);
    }
}