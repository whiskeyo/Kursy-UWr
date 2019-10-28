// Wybierz zaprogramowane wcześniej przez Ciebie dwie kolekcje. Zastanów się,
// jakie są wspólne operacje w tych kolejkach. Zaprogramuj interfejs
// zwierający nagłówki tych operacji i przebuduj tak implementacje tych
// kolekcji, aby klasy implementowały ten interfejs. Dodatkowo zaimplementuj
// w jednej z tych kolekcji interfejs IEnumerable, metodę string ToString(),
// dostęp indeksowany i właściwość int Length. 

using System;
using System.Text;
using System.Linq;
using System.Collections;               // for IEnumerable
using System.Collections.Generic;      // for IEnumerable (allows using foreach)

namespace Exercise1
{
    class Program
    {
        static void Main(string[] args)
        {
            LazyList list = new LazyList();
            int temp = list[20];
            int i = 0;

            foreach (int p in list)
            {
                Console.WriteLine(p);
                i++;
            }

            list[0] = 0;
            list[19] = 19;
            list[20] = 20;

            Console.WriteLine("\nTesting indexer:");
            Console.WriteLine("list[0] = " + list[0]);
            Console.WriteLine("list[19] = " + list[19]);
            Console.WriteLine("list[20] = " + list[20]);

            list[30] = 30;

            Console.WriteLine("list[30] = " + list[30]);
            Console.WriteLine("Last Element = " + list.ToString());
            Console.WriteLine("Length of a list = " + list.Length);
        }
    }


    interface IList
    {
        int Element(int i);
        int Size();
    }

    class LazyList : IList, IEnumerable
    {
        private int elem_count;
        List<int> int_list;
        Random rng;

        public LazyList()
        {
            this.elem_count = 0;
            this.int_list = new List<int>();
            this.rng = new Random();
        }

        public int Element(int i)
        {
            if (i > this.elem_count)
            {
                int how_many = i - this.elem_count;
                this.FillIntList(how_many);
                this.elem_count = i;
            }

            return this.int_list[i - 1];
        }

        public int Length // it is property (of the interface)
        {
            get
            {
                return Size();
            }
        }

        public int Size()
        {
            return this.elem_count;
        }

        void FillIntList(int how_many)
        {
            while (how_many > 0)
            {
                this.int_list.Add(rng.Next(-Int32.MaxValue, Int32.MaxValue));
                how_many--;
            }
        }

        public int this[int index] // indexer declaration
        {
            get // indexes of the list begin from 1, so our indexer returns given index + 1
            {
                if (index < 0)
                    throw new System.IndexOutOfRangeException();
                return Element(index + 1);
            }

            set // if there is no element which we want to write to, we create a new element in list
            {
                if (index < 0)
                    throw new System.IndexOutOfRangeException();
                
                if (index >= elem_count)
                    Element(index + 1);
                
                int_list[index] = value;
            }
        }

        public override string ToString()
        {
            if (elem_count == 0)
                return null;
            return int_list[elem_count - 1].ToString();
        }

        // These two methods are needed if we implement IEnumerable interface,
        // both of them return object implementing other IEnumerator interface.
        public IEnumerator GetEnumerator()
        {
            return new LazyListEnum(int_list);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new LazyListEnum(int_list);
        }
    }

    // IEnumerator implements 4 functions, which are:
    // public bool MoveNext(), public void Reset(),
    // public int Current, object IEnumerator.Current
    // and are needed for foreach loop to work.

    public class LazyListEnum : IEnumerator
    {
        List<int> int_list;
        int position;

        // Constructor takes our list, so functions can get values.
        public LazyListEnum(List<int> int_list)
        {
            this.int_list = int_list;
            position = -1;
        }

        // If last element returns false, then Current
        // will not call the element that does not exist.
        public bool MoveNext()
        {
            position++;
            return (position < int_list.Count);
        }

        public void Reset()
        {
            position = -1;
        }

        public int Current
        {
            get
            {
                return int_list[position];
            }
        }

        // It returns the same value as Current, similarly as in IEnumerable.
        object IEnumerator.Current
        {
            get
            {
                return Current; // it can also be return int_list[position];
            }
        }
    }

    class Prime : IList
    {
        private int elem_count;
        List<int> prime_list;
        int checker;

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
                this.checker++;
            }
        }

        public int Size()
        {
            return elem_count;
        }
    }
}