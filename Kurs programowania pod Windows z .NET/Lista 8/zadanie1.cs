using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace zadanie1
{
    class Program
    {
        static void Main(string[] args)
        {
            // w connString mamy parametry laczenia z baza danych
            string connString = "Provider=Microsoft.ACE.OLEDB.12.0; Data Source=dane.xlsx; Extended Properties='Excel 8.0;HDR=Yes'";
            using (OleDbConnection connection = new OleDbConnection(connString))
            {
                // laczymy sie i wchodzimy do bazy z pliku dane.xlsx
                connection.Open(); 

                // tworzymy adapter, a nastepnie tworzymy zapytanie
                OleDbDataAdapter adapter = new OleDbDataAdapter();
                adapter.SelectCommand = new OleDbCommand("SELECT * FROM [Arkusz1$]", connection);

                // na koncu wypelniamy zbior danych
                DataSet data = new DataSet();
                adapter.Fill(data, "Studenci");

                // i wypisujemy imie, nazwisko, pesel i indeks kazdego studenta
                foreach (var m in data.Tables[0].DefaultView)
                    Console.WriteLine(((DataRowView)m).Row.ItemArray[0] + "\t" + ((DataRowView)m).Row.ItemArray[1] + "\t" + ((DataRowView)m).Row.ItemArray[2] + "\t" + ((DataRowView)m).Row.ItemArray[3]);
            }

            Console.ReadKey();
        }
    }
}
