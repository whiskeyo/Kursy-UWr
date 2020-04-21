using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie1._1._6
{
    // uzycie klas: metody, pola, propercje, indeksery, delegacje, zdarzenia
    // podstawowe konstrukcje: petle, ify, switch

    public class Example
    {   
        // pole
        int[] numbers;

        // konstruktory
        public Example() { numbers = new int[] { 1, 2, 3 }; }
        public Example(params int[] input)
        {
            numbers = new int[input.Length];
            for (int i = 0; i < input.Length; i++)
                numbers[i] = input[i];
        }
        
        // metody
        public int addNumbers()
        {
            int sum = 0;
            for (int i = 0; i < numbers.Length; i++)
                sum += numbers[i];

            return sum;
        }

        // indeksery
        public int this[int i]
        {
            get { return numbers[i]; }
            set { numbers[i] = value; }
        }

        // wlasciwosci (propercje)
        public int numberOfElements
        {
            get { return numbers.Length; }
        }
    }

    // delegat
    public delegate void Operator(int x, int y);
    public class Maths
    {
        public void printSum(int x, int y) { Console.WriteLine(x + y); }
        public void printDiff(int x, int y) { Console.WriteLine(x - y); }
    }

    // zdarzenia
    class EventExample
    {
        public int value;
        public delegate void NumberManipulationHandler();
        public event NumberManipulationHandler ChangeValue;

        protected virtual void OnValueChanged()
        {
            if (ChangeValue != null) ChangeValue();
            else Console.WriteLine("Event occured");
        }

        public EventExample(int i) { SetValue(i); }
        public void SetValue(int i)
        {
            if (value != i)
            {
                value = i;
                OnValueChanged();
            }
        }
    }

    class Program
    {
        static void Main()
        {
            Example example = new Example(1, 2, 3, 4, 5, 6);
            Maths maths = new Maths();
            Operator ex = new Operator(maths.printSum);

            ex(5, 5); // wywolanie maths.printSum(5, 5) <- delegat

            EventExample ee = new EventExample(1);
            ee.SetValue(2);
            ee.SetValue(3);

            for (int i = 0; i < 10; i++)
                Console.WriteLine(i * i);

            bool if_test1 = true, if_test2 = false;

            if (if_test1)
                Console.WriteLine("if_test1 = true");
            else
                Console.WriteLine("if_test1 = false");

            if (if_test2)
                Console.WriteLine("if_test2 = true");
            else
                Console.WriteLine("if_test2 = false");

            int caseSwitch = 1;

            switch (caseSwitch)
            {
                case 1: Console.WriteLine("Case 1"); break;
                case 2: Console.WriteLine("Case 2"); break;
                case 3: Console.WriteLine("Case 3"); break;
                default: Console.WriteLine("Case default"); break;
            }
            Console.ReadKey(); 
        }
    }
}
