// Zaprogramuj klasę PrimeCollection implementującą interfejs IEnumerable
// (bądź IEnumerable<T>) omówiony na wykładzie. Obiekty tej klasy powinny
// być kolekcją liczb pierwszych. Jednak kolekcja ta nie powinna budować
// prawdziwej kolekcji, tylko "w locie" obliczać kolejną liczbę pierwszą.
// Program w static void Main powinien wypisywać kolejne liczby pierwsze
// aż do przekroczenia zakresu typu int.

using System;
using System.Text;
using System.Linq;
using System.Collections;               // for IEnumerable
using System.Collections.Generic;      // for IEnumerable (allows using foreach)

namespace Exercise2
{
    class Program
    {
        static void Main(string[] args)
        {
            Prime prime = new Prime();
            foreach (int p in prime)
                Console.WriteLine(p);
        }
    }

    class Prime : IEnumerable<int>, IEnumerator<int>
    {
        private int elem_count;
        List<int> prime_list;
        int checker;
        int position;
        bool inRange;

        private bool IsPrime(int number)
        {
            for (int i = 2; i < number; i++)
                if (number % i == 0)
                    return false;
            
            return true;
        }

        public Prime()
        {
            this.prime_list = new List<int>();
            this.checker = 2;
            this.position = 0;
            this.inRange = true;
        }

        public int Element(int i)
        {
            if (i > elem_count)
            {
                int how_many = i - elem_count;
                this.FillPrimeList(how_many);
                elem_count = i;
            }
            return this.prime_list[i - 1];
        }

        void FillPrimeList(int how_many)
        {
            while (how_many > 0)
            {
                if (IsPrime(checker) == true)
                {
                    this.prime_list.Add(this.checker);
                    how_many--;        
                }
                if (checker >= Int32.MaxValue)
                {
                    inRange = false;
                    return;
                }
                this.checker++;
            }
        }

        public int Size()
        {
            return elem_count;
        }

        public IEnumerator<int> GetEnumerator()
        {
            // As this class implements IEnumerator and IEnumerable
            // it can return itself in this method as an object
            // implementing IEnumerator.
            return this;
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this;
        }

        public int Current
        {
            get
            {
                return Element(position);
            }
        }

        public bool MoveNext()
        {
            position++;
            return inRange;
        }

        public void Reset()
        {
            position = 0;
        }

        public void Dispose()
        {
            
        }

        object IEnumerator.Current
        {
            get
            {
                return Current;
            }
        }
    }
}