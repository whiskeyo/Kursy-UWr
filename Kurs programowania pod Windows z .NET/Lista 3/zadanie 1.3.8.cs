using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace zadanie1_3_8
{
    class Program
    {
        public static int Puzzle(int i)
        {
            if (i <= 2)
                return 1;
            else
                return Puzzle(i - 1) + Puzzle(i - 2);
        }
        static void Main(string[] args)
        {
            List<int> list = new List<int>() { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

            Func<int, int> Y = null;
            Y = x => x <= 2 ? 1 : Y(x - 1) + Y(x - 2);

            Console.WriteLine("Z operatorem punktu stałego Y:");
            foreach (var item in list.Select(Y))
                Console.WriteLine(item);

            Console.WriteLine("Bezpośrednio (dla porównania wyników):");
            foreach (var item in list)
                Console.WriteLine(value: Puzzle(item).ToString());

            Console.ReadKey();
        }
    }
}
