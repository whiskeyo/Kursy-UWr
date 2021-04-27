using System;
using System.IO;

namespace Exercise01
{
    public interface ILogger
    {
        void Log(string Message);
    }
    public enum LogType { None, Console, File }

    public class NoneLogger : ILogger { public void Log(string Message) { } }
    public class ConsoleLogger : ILogger { public void Log(string Message) { Console.WriteLine(Message); } }
    public class FileLogger : ILogger 
    {
        private StreamWriter file;

        public FileLogger(string filePath)
        {
            if (filePath is null)
                throw new ArgumentException("File path has not been provided");

            this.file = new StreamWriter(filePath);
        }

        public void Log(string Message) { file.WriteLine(Message); }
    }

    public class LoggerFactory
    {
        private static LoggerFactory instance;
        private LoggerFactory() { }
        public static LoggerFactory Instance()
        {
            if (instance is null)
                instance = new LoggerFactory();

            return instance;
        }


        public ILogger GetLogger(LogType LogType, string Parameters = null)
        {
            switch (LogType)
            {
                case LogType.None:      return new NoneLogger();
                case LogType.Console:   return new ConsoleLogger();
                case LogType.File:      return new FileLogger(Parameters);
                default:                throw new ArgumentException("Type not found");
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            LoggerFactory factory = LoggerFactory.Instance();
            ILogger[] loggers = new ILogger[3]; 

            loggers[0] = factory.GetLogger(LogType.None);
            loggers[1] = factory.GetLogger(LogType.Console);
            loggers[2] = factory.GetLogger(LogType.File, "logs.txt");

            foreach (var logger in loggers)
                logger.Log("Test message for the logger");
        }
    }
}