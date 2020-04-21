using System;


namespace zadanie1_3_6
{
    class Program
    {
        static public int Foo(int x, int y)
        {
            return (x + y) * 3 * y / (5 * x + 2 * y) + 11 * x;
        }
        static public dynamic Foo(dynamic x, dynamic y)
        {
            return (x + y) * 3 * y / (5 * x + 2 * y) + 11 * x;
        }
        static public void TestTyped(int tests)
        {
            Random rnd = new Random();
            TimeSpan time;
            DateTime start, stop;

            start = DateTime.Now;
            for (int i = 0; i < tests; i++)
                Foo((int)rnd.Next(1, 1000), (int)rnd.Next(1, 1000));
            stop = DateTime.Now;
            time = stop - start;

            Console.WriteLine("Czas dzialania dla otypowanej funkcji: " + time);
        }
        static public void TestDynamic(int tests)
        {
            Random rnd = new Random();
            TimeSpan time;
            DateTime start, stop;

            start = DateTime.Now;
            for (int i = 0; i < tests; i++)
                Foo((double)rnd.Next(1, 1000), (float)rnd.Next(1, 1000));
            stop = DateTime.Now;
            time = stop - start;

            Console.WriteLine("Czas dzialania dla dynamicznej funkcji: " + time);
        }
        static void Main(string[] args)
        {
            TestTyped(100000000);
            TestDynamic(100000000);
            // Testy dla 100 mln wywołań funkcji otypowanej Foo wykonały się w 2-2.5 sekundy,
            // a dla dynamicznej funkcji Foo w 12-14 sekund, co oznacza, że dynamiczne typy
            // działają około 5-6 razy wolniej.
        }
    }
}
