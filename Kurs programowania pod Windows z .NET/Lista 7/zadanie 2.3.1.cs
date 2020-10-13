using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie1
{
    public class Complex : IFormattable
    {
        private double r, i;
        // 3 konstruktory dla ulatwienia pracy
        public Complex() { r = 0; i = 0; }
        public Complex(double _r) { r = _r; i = 0; }
        public Complex(double _r, double _i) { r = _r; i = _i; }
        // przeciazanie operatorow
        public static Complex operator ! (Complex a) // sprzezenie
            => new Complex(a.r, -a.i);
        public static Complex operator + (Complex a, Complex b)
            => new Complex(a.r + b.r, 
                           a.i + b.i);
        public static Complex operator - (Complex a, Complex b)
            => new Complex(a.r - b.r, 
                           a.i - b.i);
        public static Complex operator * (Complex a, Complex b)
            => new Complex(a.r * b.r - a.i * b.i, 
                           a.r * b.i + a.i * b.r);
        public static Complex operator / (Complex a, Complex b)
            => new Complex((a.r * b.r + a.i * b.i) / (b.r * b.r + b.i * b.i),
                           (a.i * b.r - a.r * b.i) / (b.r * b.r + b.i * b.i));
        /*
        public static bool operator == (Complex a, Complex b)
        {
            return a.r == b.r && a.i == b.i ? true : false;
        }

        public static bool operator != (Complex a, Complex b)
        {
            return a.r != b.r || a.i != b.i ? true : false;
        }
        */

        // wlasne formatowanie: d lub brak zwraca a+bi, w zwraca [a,b]
        public string d { get { return this.r.ToString() + "+" + this.i.ToString() + "i"; } }
        public string w { get { return "[" + this.r.ToString() + ", " + this.i.ToString() + "]"; } }
        
        public override string ToString()
        {
            return this.ToString("d", CultureInfo.CurrentCulture);
        }

        public string ToString(string format)
        {
            return this.ToString(format, CultureInfo.CurrentCulture);
        }

        public string ToString(string format, IFormatProvider formatProvider)
        {
            if (String.IsNullOrEmpty(format)) format = "d";
            if (formatProvider == null) formatProvider = CultureInfo.CurrentCulture;

            switch (format.ToLowerInvariant())
            {
                case "d": return d;
                case "w": return w;
                default: throw new FormatException(String.Format("The {0} format string is not supported.", format));
            }
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            Complex z = new Complex(4, 3);
            Console.WriteLine(String.Format("{0}", z));
            Console.WriteLine(String.Format("{0:d}", z));
            Console.WriteLine(String.Format("{0:w}", z));

            Console.ReadKey();
        }
    }
}
