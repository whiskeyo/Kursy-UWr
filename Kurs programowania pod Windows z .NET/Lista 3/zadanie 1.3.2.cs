using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

// Roznica pomiedzy parametrami operatorow: dla where/orderby uzywamy skladni SQL, dla Where/OrderBy funkcji anonimowych.

namespace zadanie1_3_2
{
    class Program
    {
        static void Main(string[] args)
        {
            System.IO.StreamReader m_file = new System.IO.StreamReader("E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3\\plik1.txt");
            List<int> m_list = new List<int>();
            String temp;

            while ((temp = m_file.ReadLine()) != null)
                m_list.Add(Int32.Parse(temp));

            var m_query_items = from x in m_list
                                where x > 100
                                orderby x descending
                                select x;

            Console.WriteLine("Zapytanie pierwsze:");
            foreach (var elem in m_query_items)
                Console.WriteLine("{0}", elem);

            Console.WriteLine("Zapytanie drugie:");
            IEnumerable<int> query = m_list.Where(x => x > 100).OrderByDescending(x => x);
            foreach (int elem in query)
                Console.WriteLine("{0}", elem);

            Console.ReadKey();
        }
    }
}
