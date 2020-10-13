using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace Zadanie_3._1._8
{
    class Program
    {
        static void Main(string[] args)
        {
            // to enkodowanie umozliwa wyswietlanie cyrylicy i alfabetu arabskiego
            // ale potrzeba jeszcze zmienic czcionke w konsoli, na taka ktora je obsluguje
            Console.OutputEncoding = Encoding.UTF8;
            List<CultureInfo> list = new List<CultureInfo>();
            list.Add(new CultureInfo("en"));
            list.Add(new CultureInfo("de"));
            list.Add(new CultureInfo("fr"));
            list.Add(new CultureInfo("ru"));
            list.Add(new CultureInfo("ar"));
            list.Add(new CultureInfo("cs"));
            list.Add(new CultureInfo("pl"));

            foreach (var language in list)
            {
                Console.WriteLine(language.EnglishName + ":");

                // wypisanie miesiecy i ich skrotow
                for (int i = 0; i < 12; i++)
                {
                    Console.WriteLine(language.DateTimeFormat.MonthNames[i] + " --- " + language.DateTimeFormat.AbbreviatedMonthNames[i]);
                }
                Console.WriteLine();

                // wypisanie dni i ich skrotow
                for (int i = 0; i < 7; i++)
                {
                    Console.WriteLine(language.DateTimeFormat.DayNames[i] + " --- " + language.DateTimeFormat.AbbreviatedDayNames[i]);
                }
                Console.WriteLine();

                // wypisanie daty
                Console.WriteLine(DateTime.Now.ToString(language));
                Console.WriteLine();
            }
            Console.ReadKey();
        }
    }
}