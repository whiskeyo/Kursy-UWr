using System;
using System.IO;
using System.Reflection;

namespace zadanie10
{
    static class FileReader
    {
        public static string GetData(string filename)
        {
            var asm = Assembly.GetExecutingAssembly();

            using (Stream stream = asm.GetManifestResourceStream($"zadanie10.{filename}"))
            {
                using (StreamReader reader = new StreamReader(stream))
                {
                    return reader.ReadToEnd();
                }
            }
        }
    }

    // plik w projekcie jest zmieniony na Embedded Resource
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(FileReader.GetData("test.txt"));
            Console.ReadKey();
        }
    }
}
