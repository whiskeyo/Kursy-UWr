using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie2
{
    public class Set : List<int>
    {
        private List<int> set;
        public Set () { set = new List<int>(); }
        public Set (List<int> elems)
        {
            set = new List<int>();
            foreach (var elem in elems)
                AddItem(elem);
        }
        
        public void AddItem(int item)
        {
            bool exists = false;
            foreach (var elem in set)
            {
                if (item == elem)
                {
                    exists = true;
                    break;
                }
            }
            if (exists == true) return;
            else set.Add(item);
        }

        public override string ToString()
        {
            string res = "";
            foreach (var item in set)
                res += item.ToString() + " ";
            return res;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            List<int> vs = new List<int> { 5, 3, 1, 0, 0, 2, 1, 3, 5, 4, 6, 7 };
            Set set = new Set(vs);
            Console.WriteLine("Elements in set: " + set.ToString());
            set.AddItem(5);
            set.AddItem(10);
            Console.WriteLine("Elements in set after adding 5 and 10: " + set.ToString());

            Console.ReadKey();
        }
    }
}
