using System;
using System.Text;
using Exercise1;

class Program
    {
        static void Main(string[] args)
        {
            OwnList<int> xs = new OwnList<int>();
            xs.addLast(9);
            xs.addLast(8);
            xs.addLast(7);
            xs.addLast(6);
            xs.addLast(5);

            xs.addFirst(4);
            xs.addFirst(3);
            xs.addFirst(2);
            xs.addFirst(1);
            xs.addFirst(0);

            xs.printList();

            xs.deleteFirst();
            xs.deleteFirst();
            xs.deleteFirst();
            xs.deleteFirst();
            xs.deleteFirst();

            Console.WriteLine("After few deletes: ");
            xs.printList();

            xs.deleteLast();
            xs.deleteLast();
            xs.deleteLast();
            xs.deleteLast();
            xs.deleteLast();

            Console.WriteLine("At the end: ");
            xs.printList();
        }
    }