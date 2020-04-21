using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace zadanie1_3_4
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Podaj sciezke folderu: ");
            string m_path = Console.ReadLine();
            string m_path_test = "E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3";

            DirectoryInfo m_dir = new DirectoryInfo(m_path);
            FileInfo[] m_info = m_dir.GetFiles();

            foreach (FileInfo f in m_info)
                Console.WriteLine("Plik {0} ma {1} bajtów.", f.Name, f.Length);

            List<long> m_lengths = new List<long>();
            foreach (FileInfo f in m_info)
                m_lengths.Add(f.Length);

            var m_length = m_lengths.Aggregate((sum, x) => sum + x);
            Console.WriteLine("Wszystkie pliki mają łącznie {0} bajtów.", m_length);

            Console.ReadKey();
        }
    }
}
