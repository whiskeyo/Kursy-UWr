using System;
using System.Reflection;

namespace zadanie1_1_3
{
    class Example
    {
        private void privateMethod()
        {
            Console.WriteLine("Metoda prywatna!!!");
        }
        public void publicMethod()
        {
            Console.WriteLine("Metoda publiczna!!!");
        }
    }
    class Program
    {
        public static void Main(string[] args)
        {
            TimeSpan m_time_priv, m_time_pub;
            DateTime m_time_start, m_time_stop;

            // Blok kodu otaczamy początkiem i końcem zegara dla ustalenia długości działania programu
            m_time_start = DateTime.Now;
            for (int i = 0; i < 5000; i++) 
                typeof(Example).GetMethod("privateMethod", BindingFlags.NonPublic | BindingFlags.Instance).Invoke(new Example(), null);

            m_time_stop = DateTime.Now;
            m_time_priv = m_time_stop - m_time_start;

            m_time_start = DateTime.Now;
            for (int i = 0; i < 5000; i++)
            {
                Example m_example = new Example();
                m_example.publicMethod();
            }
            m_time_stop = DateTime.Now;
            m_time_pub = m_time_stop - m_time_start;

            Console.WriteLine("Podsumowanie czasow dzialania:");
            Console.WriteLine("m_time_priv = " + m_time_priv);
            Console.WriteLine("m_time_pub = " + m_time_pub);
            // po kilkukrotnym uruchomieniu testow mozna zauwazyc, ze dostep do metody prywatnej zajmuje okolo 25% wiecej czasu
            Console.ReadKey();
        }
    }
}