using System;
using System.Xml;
using System.IO;
using System.Data.SqlClient; // this needs NuGet packed to be installed

namespace Exercise03
{
    public interface IDataAccessStrategy
    {
        void Connect();
        void GetData();
        void Process();
        void Close();
    }

    public class DataAccessHandler
    {
        private IDataAccessStrategy strategy;

        public DataAccessHandler(IDataAccessStrategy strategy) { this.strategy = strategy; }

        public void Execute()
        {
            this.strategy.Connect();
            this.strategy.GetData();
            this.strategy.Process();
            this.strategy.Close();
        }
    }

    public class SqlDataAccessHandler : IDataAccessStrategy
    {
        public string ConnectionString { get; }
        public string Table { get; }
        public string Column { get; }
        SqlConnection connection = null;
        int sum;

        public SqlDataAccessHandler(string connectionString, string column, string table)
        {
            this.ConnectionString = connectionString;
            this.Column = column;
            this.Table = table;
        }

        public void Connect()
        {
            connection = new SqlConnection(this.ConnectionString);
        }

        public void GetData()
        {
            var command = new SqlCommand(
                String.Format("SELECT SUM({0}) FROM {1}", Column, Table),
                connection
            );

            sum = (int)command.ExecuteScalar();
        }

        public void Process()
        {
            Console.WriteLine("SUM {0}.{1} is {2}", Table, Column, sum);
        }

        public void Close()
        {
            connection.Close();
        }
    }

    public class XmlDataAccessHandler : IDataAccessStrategy
    {
        public string FilePath { get; }
        private XmlDocument doc = null;

        public XmlDataAccessHandler(string fileName)
        {
            this.FilePath = fileName;
        }

        public void Connect() { /* nothing to be done */ }

        public void GetData()
        {
            this.doc = new XmlDocument();
            this.doc.Load(this.FilePath);
        }

        private static string LongestNodeName(XmlNodeList nodes)
        {
            var longest = "";

            foreach (XmlNode node in nodes)
            {
                if (longest.Length < node.Name.Length)
                {
                    longest = node.Name;
                }

                var childLongest = LongestNodeName(node.ChildNodes);
                if (childLongest.Length > longest.Length)
                {
                    longest = childLongest;
                }
            }

            return longest;
        }

        public void Process()
        {
            var root = this.doc.DocumentElement;
            var longest = LongestNodeName(root.ChildNodes);
            Console.WriteLine("Longest node is \"{0}\"", longest);
        }

        public void Close() { }
    }

    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}