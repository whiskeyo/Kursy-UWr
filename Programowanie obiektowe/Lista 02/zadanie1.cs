using System;
using System.Text; // uzycie StringBuilder

namespace Zadanie1
{
    class Program
    {
        static void Main(string[] args)
        {
            var primeStream = new PrimeStream();
            var randomStream = new RandomStream();
            var randomWordStream = new RandomWordStream();

            int ile_losowych = 5;
            int ile_pierwszych = 10;
            int ile_napisow = 10;

            Console.WriteLine("Liczby losowe:");
            for (int i = 0; i < ile_losowych; i++)
                Console.WriteLine(randomStream.Next());

            Console.WriteLine("Liczby pierwsze:");
            for (int i = 0; i < ile_pierwszych; i++)
                Console.WriteLine(primeStream.Next());

            Console.WriteLine("Napisy o dlugosci kolejnych liczb pierwszych:");
            for (int i = 0; i < ile_napisow; i++)
                Console.WriteLine(randomWordStream.Next());

            Console.WriteLine();
            Console.WriteLine("Nacisnij ENTER aby zakonczyc.");
            Console.ReadLine();
        }
    }

    public class IntStream
    {
        public int aktualny;

        public IntStream()
        {
            aktualny = 0;
        }

        virtual public int Next()
        {
            if (Eos() == false)
                return ++aktualny;
            else
            {
                Console.WriteLine("Przekroczono zakres, nie mozna juz zwiekszyc liczby.");
                return aktualny;   
            }
        }

        virtual public bool Eos()
        {
            if (Int32.MaxValue >= aktualny)
                return false;
            else 
                return true;
        }

        virtual public void Reset()
        {
            aktualny = 0;
        }
    }

    public class PrimeStream : IntStream
    {   
        public PrimeStream()
        {
            aktualny = 1;
        }

        private bool czy_pierwsza(int n)
        {
            for (int i = 2; i * i < n; i++)
                if (n % i == 0)
                    return false;
            return true;
        }

        public override int Next()
        {   
            if (Eos() == true)
                Console.WriteLine("Przekroczono zakres, nie mozna juz zwiekszyc liczby.");

            aktualny++;
            while (czy_pierwsza(aktualny) == false)
            {
                if (Eos() == true)
                {
                    Console.WriteLine("Przekroczono zakres, nie mozna juz zwiekszyc liczby.");
                    break;
                }
                else aktualny++;
            }
            return aktualny;
        }

        public override bool Eos()
        {
            if (Int32.MaxValue >= aktualny)
                return false;
            else
                return true;
        }

        public override void Reset()
        { 
            aktualny = 1;
        }
    }

    public class RandomStream : IntStream
    {
        private Random rand;

        public RandomStream()
        {
            rand = new Random(); // stworzenie obiektu Random o nazwie rand
        }

        public override int Next()
        {
            return rand.Next();
        }

        public override bool Eos()
        {
            return false;
        }
    }

    public class RandomWordStream
    {
        private PrimeStream pStream;
        private RandomStream rand;

        public RandomWordStream()
        {
            pStream = new PrimeStream();
            rand = new RandomStream();
        }

        public string Next()
        {
            int kolejna_pierwsza = pStream.Next();
            StringBuilder napis = new StringBuilder(); // obiekt typu StringBuilder

            for (int i = 0; i < kolejna_pierwsza; i++)
                napis.Append(Losowy_znak());

            return napis.ToString(); // konwertowanie na string
        }

        private char Losowy_znak()
        {
            int losowanie = rand.Next() % 26;
            return (char)('a' + losowanie);
        }
    }
}