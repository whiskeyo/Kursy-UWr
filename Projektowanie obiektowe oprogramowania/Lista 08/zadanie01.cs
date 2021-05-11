using System;
using System.IO;
using System.Net;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

namespace Exercise01
{
    public interface IFileCommand { void Execute(); }

    public class FTPDownloadCommand : IFileCommand
    {
        string connectionAddress;
        string fileName;

        public FTPDownloadCommand(string connectionAddress, string fileName)
        {
            this.connectionAddress = connectionAddress;
            this.fileName = fileName;
        }

        public void Execute()
        {
            var client = new WebClient();
            client.DownloadFile(this.connectionAddress, this.fileName);
        }
    }

    public class HTTPDownloadCommand : IFileCommand
    {
        string connectionAddress;
        string fileName;

        public HTTPDownloadCommand(string connectionAddress, string fileName)
        {
            this.connectionAddress = connectionAddress;
            this.fileName = fileName;
        }

        public void Execute()
        {
            var client = new WebClient();
            client.DownloadFile(this.connectionAddress, this.fileName);
        }
    }

    public class CreateFileCommand : IFileCommand
    {
        string fileName;
        int fileSize;

        public CreateFileCommand(string fileName, int fileSize)
        {
            this.fileName = fileName;
            this.fileSize = fileSize;
        }

        public void Execute()
        {
            Random random = new Random();
            Byte[] bytes = new Byte[this.fileSize];

            random.NextBytes(bytes);
            File.WriteAllBytes(fileName, bytes);
        }
    }

    public class CopyFileCommand : IFileCommand
    {
        string fileNameSrc;
        string fileNameDst;

        public CopyFileCommand(string fileNameSrc, string fileNameDst)
        {
            this.fileNameSrc = fileNameSrc;
            this.fileNameDst = fileNameDst;
        }

        public void Execute()
        {
            File.Copy(fileNameSrc, fileNameDst);
        }
    }

    public class CommandQueue
    {
        private ConcurrentQueue<IFileCommand> queue;
        private string currentDirectory;
        private int fileName = 1;

        private IFileCommand ExecuteRandom()
        {
            Random random = new Random();
            int cmdNumber = random.Next(0, 4);
            string outputFile = currentDirectory + "/" + fileName++;

            switch (cmdNumber)
            {
                case 1: return new CreateFileCommand(outputFile, 4096);
                case 2: string sourceFile = currentDirectory + "/" + random.Next(0, fileName);
                        return new CopyFileCommand(sourceFile, outputFile);
                case 3: // there should be defined some FTP command, but I do not have any
                default:
                    return new HTTPDownloadCommand("http://whiskeyo.pl", outputFile);
            }
        }

        private void Consume()
        {
            while (true)
            {
                IFileCommand command = null;
                if (this.queue.TryDequeue(out command))
                {
                    command.Execute();
                    Thread.Sleep(500);
                }
            }
        }

        public CommandQueue(string currentDirectory, Nullable<int> commandCount = null)
        {
            this.queue = new ConcurrentQueue<IFileCommand>();
            this.currentDirectory = currentDirectory;

            if (!Directory.Exists(currentDirectory))
            {
                Directory.CreateDirectory(currentDirectory);
                new CreateFileCommand(currentDirectory + "/0", 1024).Execute();
            }

            Thread consumer1 = new Thread(Consume);
            Thread consumer2 = new Thread(Consume);

            consumer1.Start();
            consumer2.Start();

            while (commandCount == null || commandCount > 0)
            {
                var command = ExecuteRandom();
                this.queue.Enqueue(command);
                Thread.Sleep(100);

                if (commandCount is not null)
                    commandCount--;
            }
        }
    }


    class Program
    {
        static void Main(string[] args)
        {
            new CommandQueue("currentDirectory", 20);   
        }
    }
}