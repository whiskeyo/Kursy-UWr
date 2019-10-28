using System;

namespace Exercise2
{
    class Program
    {
        static void Main(string[] args)
        {
            Dictionary<int, string> dict = new Dictionary<int, string>();
            dict.Add(1, "a");
            dict.Add(2, "b");
            dict.Add(3, "c");
            dict.Add(4, "d");
            dict.Add(5, "e");
            Console.WriteLine(dict.Search(1));
            Console.WriteLine(dict.Search(2));
            Console.WriteLine(dict.Search(3));
            Console.WriteLine(dict.Search(4));
            Console.WriteLine(dict.Search(5));
            Console.WriteLine(dict.Search(11));
            Console.WriteLine("Tests after deleting keys.\n");
            dict.Delete(5);
            Console.WriteLine(dict.Search(5));
            dict.Delete(4);
            Console.WriteLine(dict.Search(4));
            Console.WriteLine(dict.Search(3));
        }
    }

    public class Dictionary<K,V>
    {
        K [] keys;
        V [] values;
        int size;
        int count_elems;
        int counter;

        public Dictionary()
        {
            size = 10;
            keys = new K[size];
            values = new V[size];
            counter = 0;
            count_elems = 0;
        }

        public void Add(K key, V value)
        {
            if (counter >= 9)   // reallocates memory for more elements
            {
                Array.Resize(ref keys, keys.Length + size);
                Array.Resize(ref values, values.Length + size);
                counter = -1;   // as it increases later
            }

            keys[count_elems] = key;
            values[count_elems] = value;
            count_elems++;
            counter++;
        }

        public V Search (K key)
        {
            for (int i = 0; i < count_elems; i++)
                if (keys[i].Equals(key))
                    return values[i];
            
            return default(V);
        }

        public void Delete(K key)
        {
            int numIndex = Array.IndexOf(keys, key); // it looks for the first right element
            if (numIndex == -1)     // numIndex = -1 if there is no element found
                return;             
            
            V[] newValues = new V[count_elems - 1]; // we create two arrays in order to copy
            K[] newKeys = new K[count_elems - 1];   // them without the element 'key'

            Array.Copy(keys, 0, newKeys, 0, numIndex);  // we copy keys until we have the element
                                                        // which is going to be deleted, then
                                                        // we copy the rest of the array:
            Array.Copy(keys, numIndex + 1, newKeys, numIndex, count_elems - numIndex - 1);

            // and repeat the same procedure for values
            Array.Copy(values, 0, newValues, 0, numIndex);
            Array.Copy(values, numIndex + 1, newValues, numIndex, count_elems - numIndex - 1);

            // now we overwrite original arrays with new ones
            keys = newKeys;
            values = newValues;
            count_elems--;
        } 
    }
}