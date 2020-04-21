using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace zadanie1_3_6
{
    class Program
    {
        public class Request
        {
            public Request(string a, string b, string c, string d, string e) { time = a; ip = b; type = c; resource = d; status = e; }
            public string time { get; set; }
            public string ip { get; set; }
            public string type { get; set; }
            public string resource { get; set; }
            public string status{ get; set; }
        }

        static void Main(string[] args)
        {
            System.IO.StreamReader m_requests = new System.IO.StreamReader("E:\\MEGA.nz Cloud Storage\\Studia\\Informatyka\\Kurs programowania pod Windows z .NET\\Lista 3\\plik5.txt");

            List<Request> requests = new List<Request>();

            string temp;
            string[] data = new string[5];

            while ((temp = m_requests.ReadLine()) != null)
            {
                data = temp.Split(' ');
                requests.Add(new Request(data[0], data[1], data[2], data[3], data[4]));
            }

            /*
            foreach (Request req in requests)
                Console.WriteLine("{0} {1} {2} {3} {4}", req.time, req.ip, req.type, req.resource, req.status);
            */

            var query = (from req in requests
                        group req by req.ip into r
                        orderby r.Count() descending
                        select new { IP = r.Key, Count = r.Count() }).Take(3);

            foreach (var item in query)
                Console.WriteLine(item);

            Console.ReadKey();
        }
    }
}
