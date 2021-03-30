using System;

namespace Before
{
    // Methods in the first version, before changes, have various roles which should 
    // be in different classes. ReportPrinter should be responsible ONLY for printing
    // reports, not collecting data or formatting document.
    public class ReportPrinter
    {
        public string GetData()
        {
            Console.WriteLine("Receiving data...");
            return "Received data";
        }

        public void FormatDocument() { Console.WriteLine("Formatting document..."); }
        public void PrintReport() 
        {
            GetData();
            FormatDocument();
            Console.WriteLine("Printing report..."); 
        }
    }
}

namespace After
{
    // Classes in this version have their own roles, they are responsible for one
    // thing only. Thanks to it, all the code is easier to use in different classes
    // and is way more reusable than in the first version.
    public class ReportPrinter
    {
        public void PrintReport()
        {
            DataCollector collector = new DataCollector();
            collector.GetData();

            Formatter formatter = new Formatter();
            formatter.FormatDocument();

            Console.WriteLine("Printing report...");
        }
    }

    public class DataCollector
    {
        public string GetData()
        {
            Console.WriteLine("Receiving data...");
            return "Received data";
        }
    }

    public class Formatter
    {
        public void FormatDocument() 
        {
            Console.WriteLine("Formatting document...");
        }
    }
}

namespace Exercise02
{


    class Program
    {
        static void Main(string[] args)
        {
            var firstPrinter = new Before.ReportPrinter();
            var secondPrinter = new After.ReportPrinter();

            Console.WriteLine("Code which does not respect SRP");
            firstPrinter.PrintReport();

            Console.WriteLine("\n\nCode which respects SRP");
            secondPrinter.PrintReport();
        }
    }
}