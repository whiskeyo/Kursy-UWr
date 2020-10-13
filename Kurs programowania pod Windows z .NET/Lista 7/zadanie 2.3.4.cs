using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace zadanie4
{
    class Program
    {
        static AutoResetEvent ClientEvent = new AutoResetEvent(false);
        static ConcurrentQueue<Client> clients = new ConcurrentQueue<Client>();


        static void Main(string[] args)
        {
            Random rand = new Random();

            new Thread(Barber.CutHair) { IsBackground = true, Name = "Barber" }.Start();
            Thread.Sleep(100);

            for (int i = 0; i < 10; i++)
            {
                Thread.Sleep(rand.Next(200, 1000));

                Client c = new Client() { Name = "Client " + i };
                clients.Enqueue(c);

                if (clients.Count == 1)
                    Client.WakeUpBarber();
                else
                    Console.WriteLine(c.Name + " is waiting.");
            }
            Console.ReadKey();
        }

        class Client
        {
            public string Name { get; set; }
            public static void WakeUpBarber() => ClientEvent.Set();
        }

        class Barber
        {
            public static void CutHair()
            {
                while (!clients.IsEmpty)
                {
                    Client c;
                    Thread.Sleep(1000);

                    if (clients.TryDequeue(out c))
                    {
                        Console.WriteLine("Barber served customer {0}.", c.Name);
                    }
                }

                Console.WriteLine("Barber falls asleep.");
                GoToSleep();
            }

            private static void GoToSleep()
            {
                ClientEvent.WaitOne();
                Console.WriteLine("New client!");

                CutHair();
            }
        }
    }
}