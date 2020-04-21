using System;
using System.Collections;
using System.Collections.Generic;

namespace zadanie1_2_3
{
    public class Program
    {
        public static void Main()
        {
            List<double> m_list = new List<double>{12.44, 15.76, 7.98, 5.11, 6.45, 1.32, 20.11, 27.27};
            
            // Lista oryginalna:
            Console.WriteLine("Lista poczatkowa: ");
            foreach (var item in m_list)
                Console.Write(item + " ");

            // Zaprezentowanie metody ConvertAll:
            List<int> m_list_ints = m_list.ConvertAll(
                new Converter<double, int>(x => { return (int)x; }));
            Console.WriteLine("\nLista po ConvertAll do int: ");
            foreach (var item in m_list_ints)
                Console.Write(item + " ");

            // Zaprezentowanie metody FindAll:
            List<double> m_list_found = m_list.FindAll(x => x >= 10 && x <= 20);
            Console.WriteLine("\nLista po FindAll (przedzial od 10 do 20): ");
            foreach (var item in m_list_found)
                Console.Write(item + " ");

            // Zaprezentowanie metody ForEach:
            Console.WriteLine("\nLista poczatkowa po dodaniu 10 do kazdego elementu (tylko na teraz): "); 
            m_list.ForEach(delegate (double x) {x += 10; Console.Write(x + " ");});

            // Zaprezentowanie metody RemoveAll:
            int m_list_removed = m_list.RemoveAll(x => x < 15);
            Console.WriteLine("Lista po RemoveAll ma {0} elementow.", m_list_removed); 
            
            // Zaprezentowanie metody Sort:
            m_list_ints.Sort(
                (x, y) => {
                    if (x < y)
                        return -1;
                    else if (x > y)
                        return 1;
                    else return 0;
                }
            );
            Console.WriteLine("\nLista intow po sortowaniu: ");
            foreach (var item in m_list_ints)
                Console.Write(item + " ");

            Console.ReadKey();
        }   
    }
}