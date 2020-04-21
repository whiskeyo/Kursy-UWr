using System;
using System.Collections;
using System.Collections.Generic;

namespace zadanie1_2_4
{
    public class Program
    {
        public class ListHelper
        {
            // zmiana typu z T na TOutput za pomoca converter dla wszystkich elementow listy list
            public static List<TOutput> ConvertAll<T, TOutput>(List<T> list, Converter<T, TOutput> converter)
            {
                if (converter == null)
                    throw new System.ArgumentNullException("Parametr converter nie moze byc nullem.");

                List<TOutput> result = new List<TOutput>(list.Count);
                for (int i = 0; i < list.Count; i++)
                    result.Add(converter(list[i]));

                return result;
            }

            // znalezienie wszystkich elementow listy list spelniajacych predykat match
            public static List<T> FindAll<T>(List<T> list, Predicate<T> match)
            {
                if (match == null)
                    throw new System.ArgumentNullException("Parametr match nie moze byc nullem.");

                List<T> result = new List<T>();
                for (int i = 0; i < list.Count; i++)
                    if (match(list[i]))
                        result.Add(list[i]);

                return result;
            }

            // wykonanie akcji action dla wszystkich elementow listy list
            public static void ForEach<T>(List<T> list, Action<T> action)
            {
                if (action == null)
                    throw new System.ArgumentNullException("Parametr action nie moze byc nullem.");

                for (int i = 0; i < list.Count; i++)
                    action(list[i]);
            }

            // usuniecie elementow z listy list spelniajacych predykat match
            // zwraca liczbe usunietych elementow
            public static int RemoveAll<T>(List<T> list, Predicate<T> match)
            {
                if (match == null)
                    throw new System.ArgumentNullException("Parametr match nie moze byc nullem.");

                int m_free_index = 0; // pierwszy wolny element w liscie
                while (m_free_index < list.Count && !match(list[m_free_index]))
                    m_free_index++;

                if (m_free_index >= list.Count)
                    return 0;

                int m_current = m_free_index + 1;
                while (m_current < list.Count)
                {
                    while (m_current < list.Count && match(list[m_current]))
                        m_current++;

                    if (m_current < list.Count)
                        list[m_free_index++] = list[m_current++];
                }

                T[] temp = list.ToArray();
                Array.Clear(temp, m_free_index, list.Count - m_free_index);
                int result = list.Count - m_free_index;

                return result;
            }

            // sortowanie listy list w sposob ustalony przez comparison
            public static void Sort<T>(List<T> list, Comparison<T> comparison)
            {
                if (comparison == null)
                    throw new System.ArgumentNullException("Parametr comparison nie moze byc nullem.");

                if (list.Count > 0)
                {
                    T[] temp = list.ToArray();
                    Array.Sort(temp, 0, list.Count, Comparer<T>.Create(comparison));
                    list.Clear();

                    for (int i = 0; i < temp.Length; i++)
                        list.Add(temp[i]);
                }
            }
        }

        public static void Main()
        {
            Converter<double, int> conv = new Converter<double, int>(x => { return (int)x; });                      // convertall
            Predicate<double> match1 = new Predicate<double>(x => x >= 10 && x <= 20);                              // findall
            Action<double> act = new Action<double>(delegate (double x) { x += 10; Console.Write(x + " "); });      // foreach
            Predicate<double> match2 = new Predicate<double>(x => x < 15);                                          // removeall
            Comparison<int> comp = new Comparison<int>(                                                             // sort
                (x, y) => {
                    if (x < y)
                        return -1;
                    else if (x > y)
                        return 1;
                    else return 0;
                }
            );

            List<double> m_list = new List<double> { 12.44, 15.76, 7.98, 5.11, 6.45, 1.32, 20.11, 27.27 };
            // Lista oryginalna:
            Console.WriteLine("Lista poczatkowa: ");
            foreach (var item in m_list)
                Console.Write(item + " ");

            // Zaprezentowanie metody ConvertAll:
            List<int> m_ints = ListHelper.ConvertAll(m_list, conv);
            Console.WriteLine("\nLista po ConvertAll do int: ");
            foreach (var item in m_ints)
                Console.Write(item + " ");

            // Zaprezentowanie metody FindAll:
            List<double> m_list_found = ListHelper.FindAll(m_list, match1);
            Console.WriteLine("\nLista po FindAll (przedzial od 10 do 20): ");
            foreach (var item in m_list_found)
                Console.Write(item + " ");

            // Zaprezentowanie metody ForEach:
            Console.WriteLine("\nLista poczatkowa po dodaniu 10 do kazdego elementu (tylko na teraz): ");
            ListHelper.ForEach(m_list, act);

            // Zaprezentowanie metody RemoveAll:
            int m_list_removed = ListHelper.RemoveAll(m_list, match2);
            Console.WriteLine("\nLista po RemoveAll ma {0} elementow.", m_list_removed);

            // Zaprezentowanie metody Sort:
            ListHelper.Sort(m_ints, comp);
            Console.WriteLine("Lista intow po sortowaniu: ");
            foreach (var item in m_ints)
                Console.Write(item + " ");

            Console.ReadKey();
        }
    }
}