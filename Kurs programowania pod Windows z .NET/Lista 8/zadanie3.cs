using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace zadanie3
{
    class Program
    {
        static void Main(string[] args)
        {
            using (var dane = new zadanie2DataContext(@"server=.\sqlexpress;database=zadanie2;integrated security=true"))
            {
                // wypisywanie wszystkich studentow z tabeli
                foreach (var student in dane.Student)
                    Console.WriteLine($"{student.ID} {student.Imię} {student.Nazwisko} {student.DataUrodzenia} {student.Miejscowosc.Nazwa}");

                // dodawanie studenta do bazy (czyli stworzenie nowej klasy + wstawienie)
                Student student1 = new Student();
                student1.Imię = "Janek";
                student1.Nazwisko = "Nowacki";
                student1.DataUrodzenia = DateTime.Parse("03/11/1992");
                student1.Miejscowosc_ID = 4;
                // wstawienie danych do tabeli i zatwierdzenie zmian
                dane.Student.InsertOnSubmit(student1);
                dane.SubmitChanges();

                // dodawanie miasta, analogicznie do dodawania studenta
                Miejscowosc miejscowosc1 = new Miejscowosc();
                miejscowosc1.Nazwa = "Warszawa";
                // wstawienie danych do tabeli i zatwierdzenie zmian
                dane.Miejscowosc.InsertOnSubmit(miejscowosc1);
                dane.SubmitChanges();

                // dodawanie dwóch naraz - najpierw dane studenta, pozniej tworzymy miasto, wstawiamy je, a nastepnie
                // przypisujemy do studenta i wstawiamy studenta, na koniec zatwierdzamy wszystkie zmiany
                Student student2 = new Student();
                student2.Imię = "Adrian";
                student2.Nazwisko = "Głowa";
                student2.DataUrodzenia = DateTime.Parse("11/06/1993");
                Miejscowosc miejscowosc2 = new Miejscowosc();
                miejscowosc2.Nazwa = "Gdańsk";
                dane.Miejscowosc.InsertOnSubmit(miejscowosc2);
                student2.Miejscowosc = miejscowosc2;
                dane.Student.InsertOnSubmit(student2);
                dane.SubmitChanges();

                // modyfikowanie imienia studenta o ID 3
                var dozmiany = dane.Student.Where(x => x.ID == 3).Take(1).Select(x => x);
                dozmiany.Imię = "Arnold";
                dane.SubmitChanges();

                // usuwanie studenta z imieniem Jan (pierwszego wyszukanego w tabeli)
                var dousuniecia = dane.Student.Where(x => x.Imię == "Jan").Skip(1).Take(1).Select(x => x);
                if (dousuniecia.Count() > 0)
                {
                    dane.Student.DeleteOnSubmit(dousuniecia.First());
                    dane.SubmitChanges();
                }
            }
            Console.ReadKey();
        }
    }
}
