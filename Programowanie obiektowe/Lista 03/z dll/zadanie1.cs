using System;
using System.Text;

namespace Exercise1
{
    public class OwnList<T>
    {
        class Elem
        {
            public T value;
            public Elem next;
            public Elem prev;

            public Elem(T value)
            {
                this.prev = null;
                this.next = null;
                this.value = value;
            }
        }

        Elem first;

        public OwnList()
        {
            first = null;
        }

        public bool isEmpty()
        {
            if (first == null)
                return true;
            else
                return false;
        }

        public void addFirst(T val)
        {
            Elem newElem = new Elem(val);

            if (first == null) // empty list
            {
                first = newElem;
                return;
            }
            else if (first.next == null) // list with 1 element
            {
                newElem.prev = first;
                newElem.next = first;
                first.prev = newElem;
                first = newElem;
                return;
            }
            else // list with more elements than 2
            {
                newElem.next = first; // new elem points at the first elem
                newElem.prev = first.prev; // new elem points at the last elem
                first.prev = newElem; // first.prev elem points at new elem
                first = newElem; // new became first elem
                return;
            }
        }

        public T deleteFirst()
        {
            if (first == null)
                throw new OwnException("List is empty, there is nothing to delete.");

            else
            {
                if (first.next == null)
                {
                    T val_return = first.value;
                    first = null; // garbage collector
                    return val_return;
                }
                else
                {
                    T val_return = first.value;
                    first.next.prev = first.prev;   // 2nd elem points at the last elem
                                                    // as it's going to be the 1st elem
                    first = first.next;          // garbage collector
                    return val_return;
                }
            }
        }

        public void addLast(T val)
        {
            Elem newElem = new Elem(val);

            if (first == null) // empty list
            {
                first = newElem;
                return;
            }
            else if (first.next == null) // list with 1 element
            {
                first.prev = newElem;
                first.next = newElem;
                newElem.prev = first;
                return;
            }
            else // list with more elements than 2
            {
                first.prev.next = newElem;  // last elem points at the new elem
                newElem.prev = first.prev;  // new elem points at the first elem
                first.prev = newElem;       // first elem points at the new
                                            // which means it became the last elem
                return;
            }
        }

        public T deleteLast()
        {
            if (first == null)
                throw new OwnException("List is empty, there is nothing to delete.");

            else
            {
                if (first.next == null)
                {
                    T val_return = first.value;
                    first = null; // garbage collector
                    return val_return;
                }
                else
                {
                    T val_return = first.prev.value;        // value of the last elem
                    first.prev.prev.next = null;            // first before last elem does no longer
                                                            // point at the last elem, so 
                                                            // first.prev.prev is the first before last
                    first.prev = first.prev.prev;           // first elem points at the first before last
                                                            // elem, so it becomes the last elem
                    return val_return;                                              
                }   
            }
        }

        public void printList()
        {
            Elem temp = first;
            while (temp != null)
            {
                Console.WriteLine(temp.value);
                temp = temp.next;
            }
        }
    }

    public class OwnException : System.Exception
    {
        public OwnException(string warning) : base(warning)
        {
            this.Source = "Unknown exception :(";
            // this.Message = Message; is given by : base(warning)
        }
    }
}