using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace zadanie1_3_5
{
    class Program
    {
        public class Person
        {
            public Person(string n, string s, long p) { Name = n; Surname = s; PESEL = p; }
            public string Name { get; set; }
            public string Surname { get; set; }
            public long PESEL { get; set; }
        }

        public class Account
        {
            public Account(long p, long a) { PESEL = p; AccNo = a; }
            public long PESEL { get; set; }
            public long AccNo { get; set; }
        }

        static void Main(string[] args)
        {
            System.IO.StreamReader m_data = new System.IO.StreamReader("E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3\\plik3.txt");
            System.IO.StreamReader m_accs = new System.IO.StreamReader("E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3\\plik4.txt");

            List<Person> m_people = new List<Person>();
            List<Account> m_accounts = new List<Account>();

            string temp;
            string[] data = new string[3];

            while ((temp = m_data.ReadLine()) != null)
            {
                data = temp.Split(' ');
                m_people.Add(new Person(data[0], data[1], (long)Convert.ToInt64(data[2])));
            }

            string[] accs = new string[2];
            while ((temp = m_accs.ReadLine()) != null)
            {
                accs = temp.Split(' ');
                m_accounts.Add(new Account((long)Convert.ToInt64(accs[0]), (long)Convert.ToInt64(accs[1])));
            }

            /*
            foreach (Person per in m_people)
                Console.WriteLine("{0} {1} {2}", per.Name, per.Surname, per.PESEL);

            foreach (Account acc in m_accounts)
                Console.WriteLine("{0} {1}", acc.PESEL, acc.AccNo);
            */

            var query = from per in m_people
                        join acc in m_accounts on per.PESEL equals acc.PESEL
                        select new { Imie = per.Name, Nazwisko = per.Surname, PESEL = per.PESEL, NumerKonta = acc.AccNo };

            foreach (var item in query)
                Console.WriteLine(item);

            Console.ReadKey();
        }
    }
}
