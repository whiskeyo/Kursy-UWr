using System;
using System.Collections.Generic; // uzycie List<T>

namespace Zadanie2
{
    class Program
    {
        static void Main(string[] args)
        {
            ListaLeniwa lista = new ListaLeniwa();
            Pierwsze pierwsze = new Pierwsze();

            int switcher1 = 0, switcher2 = 0, elementy = 1;
            while (switcher1 != 3)
            {
                Console.WriteLine("Wybierz jedna z opcji:");
                Console.WriteLine("1. Losowe liczby");
                Console.WriteLine("2. Lista Liczb pierwszych");
                Console.WriteLine("3. Zakoncz dzialanie programu");

                switcher1 = Convert.ToInt32(Console.ReadLine());
                switch (switcher1)
                {
                    case 1:
                        Console.WriteLine("1. Ile elementow jest w liscie?");
                        Console.WriteLine("2. Wypisz element (moze rozszerzyc liste)");

                        switcher2 = Convert.ToInt32(Console.ReadLine());
                        switch(switcher2)
                        {
                            case 1:
                                Console.WriteLine("Ilosc elementow w liscie: {0}", lista.size());
                                break;
                            
                            case 2:
                                Console.WriteLine("Podaj liczbe rozna od 0: ");
                                elementy = Convert.ToInt32(Console.ReadLine());
                                Console.WriteLine(lista.element(elementy));
                                break;
                            
                            default:
                                Console.WriteLine("Nie ma takiej opcji.");
                                break;
                        }
                        break;

                    case 2:
                        Console.WriteLine("1. Ile elementow jest w liscie?");
                        Console.WriteLine("2. Wypisz element (moze rozszerzyc liste)");

                        switcher2 = Convert.ToInt32(Console.ReadLine());
                        switch(switcher2)
                        {
                            case 1:
                                Console.WriteLine("Ilosc elementow w liscie: {0}", pierwsze.size());
                                break;
                            
                            case 2:
                                Console.WriteLine("Podaj liczbe rozna od 0: ");
                                elementy = Convert.ToInt32(Console.ReadLine());
                                Console.WriteLine(pierwsze.element(elementy));
                                break;
                            
                            default:
                                Console.WriteLine("Nie ma takiej opcji.");
                                break;
                        }
                        break;

                    case 3:
                        break;
                    
                    default:
                        Console.WriteLine("Nie ma takiej opcji.");
                        break;
                }

                Console.WriteLine("Aby kontynuowac nacisnij ENTER. (terminal zostanie wyczyszczony)");
                Console.ReadLine();
                Console.Clear();
            }
        }
    }

    class ListaLeniwa
    {
        public int liczba_elem;
        List<int> lista_int;
        Random rand;

        public ListaLeniwa()
        {
            this.liczba_elem = 0;
            this.lista_int = new List<int>();
            this.rand = new Random();
        }

        virtual public int element(int i)
        {
            if (i > this.liczba_elem)
            {
                int ile = i - this.liczba_elem;
                this.uzupelnij_liste_int(ile);
                this.liczba_elem = i;
            }

            return this.lista_int[i - 1];
        }

        virtual public int size()
        {
            return this.liczba_elem;
        }

        void uzupelnij_liste_int(int ile)
        {
            while (ile > 0)
            {
                this.lista_int.Add(rand.Next(-2147000000, 2147000000));
                ile--;
            }
        }
    }

    class Pierwsze : ListaLeniwa
    {
        List<int> lista_pierwszych;
        int elem;

        public Pierwsze()
        {
            this.lista_pierwszych = new List<int>();
            this.elem = 2;
        }

        private bool czy_pierwsza(int n)
        {
            for (int i = 2; i < n; i++)
                if (n % i == 0)
                    return false;
            return true;
        }

        override public int element(int i)
        {
            if (i > liczba_elem)
            {
                int ile = i - liczba_elem;
                this.uzupelnij_liste_pierwszych(ile);
                liczba_elem = i;
            }

            return this.lista_pierwszych[i-1];
        }

        void uzupelnij_liste_pierwszych(int ile)
        {
            while (ile > 0)
            {
                if (czy_pierwsza(elem) == true)
                {
                    this.lista_pierwszych.Add(this.elem);
                    ile--;
                }
                this.elem++;
            }
        }

        override public int size()
        {
            return liczba_elem;
        }
    }
}