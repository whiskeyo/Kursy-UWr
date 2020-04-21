using System;
using System.Reflection;

namespace zadanie1_1_4
{

    internal class OznaczonoAttribute : Attribute {}

    public class Foo
    {
        [Oznaczono]
        public int metodaA() { return 3; }

        [Oznaczono]
        public char metodaB() { return 'a'; }

        public bool metodaC() { return true; }

        [Oznaczono]
        public int metodaD() { return 10; }

        [Oznaczono]
        public double metodaE() { return 1.223; }

        [Oznaczono]
        static public int metodaF() { return 112233; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Type m_type = (typeof(Foo));
            MethodInfo[] m_info = m_type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly);

            for (int i = 0; i < m_info.Length; i++)
            {
                if (m_info[i].ReturnType == typeof(Int32))
                    Console.WriteLine(m_info[i]);
            }

            Console.ReadKey();
        }
    }
}