using System;
using Exercise2;

class Program
    {
        static void Main(string[] args)
        {
            Dictionary<int, string> dict = new Dictionary<int, string>();
            dict.Add(1, "a");
            dict.Add(2, "b");
            dict.Add(3, "c");
            dict.Add(4, "d");
            dict.Add(5, "e");
            Console.WriteLine(dict.Search(1));
            Console.WriteLine(dict.Search(2));
            Console.WriteLine(dict.Search(3));
            Console.WriteLine(dict.Search(4));
            Console.WriteLine(dict.Search(5));
            Console.WriteLine(dict.Search(11));
            Console.WriteLine("Tests after deleting keys.\n");
            dict.Delete(5);
            Console.WriteLine(dict.Search(5));
            dict.Delete(4);
            Console.WriteLine(dict.Search(4));
            Console.WriteLine(dict.Search(3));
        }
    }