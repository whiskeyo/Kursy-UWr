using System;
using System.Text.RegularExpressions;
using System.Linq;

namespace zadanie1_3_1
{
    public static class StringExtensions
    {
        public static bool IsPalindromeLINQ(this string s)
        {
            s = Regex.Replace(s, @"[\s.!?\\-]+", "").ToLower();
            return s.SequenceEqual(s.Reverse());
        }

        public static bool IsPalindrome (this string s)
        {
            s = Regex.Replace(s, @"[\s.!?\\-]+", "").ToLower();
            string rev = s.Reverse();
            if (s.Equals(rev))
                return true;
            return false;
        }

        public static string Reverse(this string s)
        {
            char[] temp = s.ToCharArray();
            Array.Reverse(temp);
            return new string(temp);
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            string a = "Kobyła ma mały bok.";
            string b = "kajak";
            string c = "halko";

            bool[] tests = new bool[] { a.IsPalindrome(), b.IsPalindrome(), c.IsPalindrome() };
            bool[] testsLINQ = new bool[] { a.IsPalindromeLINQ(), b.IsPalindromeLINQ(), c.IsPalindromeLINQ() };
            Console.WriteLine("a: {0}, b: {1}, c: {2}", tests[0], tests[1], tests[2]);
            Console.WriteLine("a: {0}, b: {1}, c: {2}", testsLINQ[0], testsLINQ[1], testsLINQ[2]);
            Console.ReadKey();
        }
    }
}
