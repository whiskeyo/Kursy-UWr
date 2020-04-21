using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace zadanie_1._1._1
{
    class Program
    {
        static void Main(string[] args)
        {
            for (int i = 1; i <= 100000; i++)
            {
                var digits = new Stack<int>();  // digits - stos przechowuj�cy wszystkie cyfry liczby i
                int sumOfDigits = 0, j = i;     
                bool printable = true;

                do // dla liczby j=i uzyskujemy kolejne cyfry poprzez wykorzystanie dzielenia modulo oraz calkowitego
                {
                    sumOfDigits += (j % 10);
                    digits.Push(j % 10);
                    j /= 10;
                } while (j > 0);
                int[] digitsArray = digits.ToArray(); // stos zmieniamy na list� int�w

                for (int k = 0; k < digitsArray.Length; k++) // dla ka�dej cyfry z listy digitsArray sprawdzamy, czy jest ona dzielnikiem naszej liczby i
                {
                    if ((digitsArray[k] != 0) && (i % digitsArray[k] != 0)) // jezeli cyfra to 0 lub reszta z dzielenia jest rozna od 0, to liczby nie wyswietlamy i konczymy sprawdzanie
                    {
                        printable = false;
                        break;
                    }
                }

                if (printable && i % sumOfDigits == 0) // jesli liczba jest podzielna przez kazda ze swoich cyfr oraz przez ich sume, wyswietlamy liczbe
                    Console.WriteLine("{0}", i);
            }
            Console.ReadKey();
        }
    }
}
