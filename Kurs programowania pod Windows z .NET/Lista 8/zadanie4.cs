using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data.Entity;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie4
{
    class Program
    {
        static void Main(string[] args)
        {
            using(var dane = new zadanie4dbContext())
            {
                // wszystkie polecenia dzialaja podobnie jak w poprzednim zadaniu
                // dodawanie dwóch naraz
                Student student1 = new Student();
                student1.Imię = "Janek";
                student1.Nazwisko = "Nowacki";
                student1.DataUrodzenia = DateTime.Parse("03/11/1992");
                Miejscowosc miejscowosc1 = new Miejscowosc();
                miejscowosc1.Nazwa = "Warszawa";
                student1.Miejscowosc = miejscowosc1;
                // wstawienie i zapisanie zmian
                dane.Miejscowosc.Add(miejscowosc1);
                dane.Student.Add(student1);
                dane.SaveChanges();

                // modyfikacja miejscowości dla pierwszego studenta na poprzednio dodaną
                var student2 = dane.Student.First();
                student2.Miejscowosc = miejscowosc1;
                dane.SaveChanges();

                // usuwanie miejscowości 
                var police = dane.Miejscowosc.Where(x => x.Nazwa == "Police").FirstOrDefault();
                dane.Miejscowosc.Remove(police);
                dane.SaveChanges();
            }
            Console.ReadKey();
        }
    }

    public class zadanie4dbContext : DbContext
    {
        public zadanie4dbContext() : base("cs") { }
        public zadanie4dbContext(string connectionString) : base(connectionString) { }
        public DbSet<Student> Student { get; set; }
        public DbSet<Miejscowosc> Miejscowosc { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);
            modelBuilder.Entity<Miejscowosc>().Property(miejscowosc => miejscowosc.Nazwa).IsRequired().HasMaxLength(30);
            modelBuilder.Entity<Miejscowosc>().HasMany(miejscowosc => miejscowosc.Students).WithRequired(student => student.Miejscowosc).HasForeignKey(student => student.MID);
            modelBuilder.Entity<Miejscowosc>().ToTable("Miejscowosci");
            modelBuilder.Entity<Student>().ToTable("Studenci");
            modelBuilder.Entity<Student>().Property(student => student.Imie).IsRequired().HasMaxLength(30);
            modelBuilder.Entity<Student>().Property(s => s.Nazwisko).IsRequired().HasMaxLength(30);
        }
    }

    public class Student
    {
        public int ID { get; set; }
        public string Imie { get; set; }
        public string Nazwisko { get; set; }
        public DateTime DataUrodzenia { get; set; }
        public virtual Miejscowosc Miejscowosc { get; set; }
        public int MID { get; set; }
    }

    public class Miejscowosc
    {
        public int ID { get; set; }
        public string Nazwa { get; set; }
        public ICollection<Student> Students { get; set; }
    }
}
