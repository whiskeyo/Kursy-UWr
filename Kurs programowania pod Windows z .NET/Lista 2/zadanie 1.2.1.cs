using System;
using System.Collections;
using System.Collections.Generic;

namespace zadanie1_2_1
{
    class Tester
    {
        private TimeSpan m_time;
        private DateTime m_time_start, m_time_stop;
        private int m_sum;
        private int m_tests;

        public Tester(int m_tests) { this.m_tests = m_tests; }

        private void startTime()
        {
            m_sum = 0;
            m_time_start = DateTime.Now;
        }

        private void stopTime(string l_structure_name)
        {
            m_time_stop = DateTime.Now;
            m_time = m_time_stop - m_time_start;
            Console.WriteLine("Czas operacji struktury " + l_structure_name + " wynosi " + m_time);
        }

        public void TDictionary()
        {
            startTime();
            Dictionary<int, int> m_dict = new Dictionary<int, int>();
            for (int i = 0; i < m_tests; i++) m_dict.Add(i, m_tests - i);
            for (int i = 0; i < m_tests; i++) m_sum += m_dict[i];
            for (int i = 0; i < m_tests; i++) m_dict.Remove(i);
            stopTime("Dictionary");
        }

        public void THashtable()
        {
            startTime();
            Hashtable m_hashtable = new Hashtable();
            for (int i = 0; i < m_tests; i++) m_hashtable.Add(i, m_tests - i);
            for (int i = 0; i < m_tests; i++) m_sum += (int)m_hashtable[i];
            for (int i = 0; i < m_tests; i++) m_hashtable.Remove(i);
            stopTime("Hashtable");
        }

        public void TList()
        {
            startTime();
            List<int> m_list = new List<int>();
            for (int i = 0; i < m_tests; i++) m_list.Add(i);
            foreach (int val in m_list)       m_sum += val;
            for (int i = 0; i < m_tests; i++) m_list.Remove(i);
            stopTime("List");
        }
        public void TArrayList()
        {
            startTime();
            ArrayList m_arraylist = new ArrayList();
            for (int i = 0; i < m_tests; i++) m_arraylist.Add(i);
            foreach (int val in m_arraylist)  m_sum += val;
            for (int i = 0; i < m_tests; i++) m_arraylist.Remove(i);
            stopTime("ArrayList");
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Test polega na dodaniu elementow do struktury, zsumowaniu ich (przejrzeniu), a nastepnie usunieciu z pamieci.");

            Tester m_tester1 = new Tester(50000);
            m_tester1.TList();
            m_tester1.TArrayList();

            Tester m_tester2 = new Tester(10000000);
            m_tester2.TDictionary();
            m_tester2.THashtable();

            Console.ReadKey();
            // po kilkukrotnym przeprowadzeniu testow dla 50000 elementow przy List oraz ArrayList uzyskujemy lepszy czas dla List
            // a dla 10000000 elementow (okolo 1GB w pamieci) dla Dictionary i Hashtable mamy lepszy czas dla Dictionary 
            // w obu przypadkach jest to mniej wiecej czas czterokrotnie lepszy
        }
    }
}