using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace zadanie1_3_3
{
    class Program
    {
        static void Main(string[] args)
        {
            System.IO.StreamReader m_file = new System.IO.StreamReader("E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3\\plik2.txt");
            List<string> m_list = new List<string>();
            String temp;

            while ((temp = m_file.ReadLine()) != null)
                m_list.Add(temp);

            var m_query = from x in m_list
                          group x by x[0].ToString() into set
                          orderby set.Key ascending
                          select set.Key;

            foreach (var item in m_query)
                Console.WriteLine(item);

            Console.ReadKey();
        }
    }
}
