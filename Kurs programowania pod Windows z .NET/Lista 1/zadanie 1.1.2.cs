using System;

namespace zadanie_1._1._2
{
    /// <summary>
    /// Klasa Grid tworzy siatkę dwuwymiarową o wymiarach n x m, w której można przechowywać liczby całkowite.
    /// </summary>
    public class Grid
    {
        int n, m;
        int[,] elements;

        /// <summary>
        /// Konstruktor klasy <c>Grid</c>.
        /// </summary>
        /// <param name="n">Liczba wierszy siatki</param>
        /// <param name="m">Liczba kolumn siatki</param>
        public Grid(int n, int m)
        {
            this.n = n;
            this.m = m;
            elements = new int[this.n, this.m];
        }
        
        /// <summary>
        /// Indekser zwracający tablicę elementów i-tego wiersza.
        /// </summary>
        /// <param name="i">Indeks wiersza</param>
        /// <returns>Tablica int[] zawierająca elementy wiersza.</returns>
        public int[] this[int i]
        {
            get
            {
                int[] values = new int[m];
                for (int j = 0; j < m; j++)
                    values[j] = elements[i, j];

                return values;
            }
        }
        
        /// <summary>
        /// Indekser służący do ustawiania wartości siatki lub uzyskiwania ich.
        /// </summary>
        /// <param name="i">Indeks wiersza</param>
        /// <param name="j">Indeks siatki</param>
        public int this[int i, int j]
        {
            get => elements[i-1, j-1];
            set => elements[i-1, j-1] = value;
        }
    }

    /// <summary>
    /// Główna klasa programu.
    /// </summary>
    public class Program
    {
        /// <summary>
        /// Metoda główna programu prezentująca rozwiązanie zadania 1.1.2.
        /// </summary>
        public static void Main()
        {
            
            Grid grid1 = new Grid(4, 4);
            for (int i = 1; i <= 4; i++)
                for (int j = 1; j <= 4; j++)
                    grid1[i, j] = i * j;

            int[] rowdata = grid1[1];

            foreach (var item in rowdata)
                Console.WriteLine(item);
            
            Grid grid2 = new Grid(4, 4);
            for (int i = 1; i <= 4; i++)
                for (int j = 1; j <= 4; j++)
                    grid2[i, j] = i * j;

            grid2[2, 2] = 5;
            int elem = grid2[1, 3];

            Console.WriteLine(elem.ToString());

            Console.ReadKey(); 
        }
    }
}