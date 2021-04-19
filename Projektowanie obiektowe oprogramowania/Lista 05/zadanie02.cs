using System;
using System.IO;
using System.Text;

namespace Exercise02
{
    class CaesarStream : Stream
    {
        Stream stream;
        int offset;

        public CaesarStream(Stream stream, int offset) 
        { 
            this.stream = stream; 
            this.offset = offset;
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            var readStream = this.stream.Read(buffer, offset, count);

            for (int i = 0; i < buffer.Length; i++)
            {
                char ch = (char)(buffer[i]);
                if (char.IsLetter(ch) && char.IsLower(ch))
                {
                    int filler = (ch - 97 + this.offset) < 0 ? 26 : 0;
                    buffer[i] = (byte)((ch - 97 + this.offset + filler) % 26 + 97);
                }
                else if (char.IsLetter(ch) && char.IsUpper(ch))
                {
                    int filler = (ch - 65 + this.offset) < 0 ? 26 : 0;
                    buffer[i] = (byte)((ch - 65 + this.offset + filler) % 26 + 65);
                }
                else
                    buffer[i] = buffer[i];
            }

            return readStream;
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            var newBuffer = new byte[buffer.Length];

            for (int i = 0; i < buffer.Length; i++)
            {
                char ch = (char)(buffer[i]);
                if (char.IsLetter(ch) && char.IsLower(ch))
                    newBuffer[i] = (byte)((buffer[i] + this.offset - 97) % 26 + 97);
                else if (char.IsLetter(ch) && char.IsUpper(ch))
                    newBuffer[i] = (byte)((buffer[i] + this.offset - 65) % 26 + 65);
                else
                    newBuffer[i] = buffer[i];
            }

            this.stream.Write(newBuffer, offset, count);
        }

        public override bool CanRead  => stream.CanRead;
        public override bool CanSeek  => stream.CanSeek;
        public override bool CanWrite => stream.CanWrite;
        public override long Length   => stream.Length;
        public override long Position { get => stream.Position; set => stream.Position = value; }
        public override void Flush() { stream.Flush(); }
        public override long Seek(long offset, SeekOrigin origin) { return stream.Seek(offset, origin); }
        public override void SetLength(long value) { stream.SetLength(value); }
    }

    class Program
    {
        static void Main(string[] args)
        {
            const string input = "abcdefghijklmnopqrstuvwxyz " +
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ " +
                "1234567890 " +
                "!@#$%";

            FileStream fileToWrite = File.Create("input.txt");
            CaesarStream caeToWrite = new CaesarStream(fileToWrite, 5);
            caeToWrite.Write(Encoding.ASCII.GetBytes(input));
            fileToWrite.Close();
            Console.WriteLine("before:   '{0}'", input);

            FileStream fileToRead = File.OpenRead("input.txt");
            CaesarStream caeToRead = new CaesarStream(fileToRead, -5);
            var bytes = new byte[1000];
            caeToRead.Read(bytes);
            Console.WriteLine("after:    '{0}'", Encoding.ASCII.GetString(bytes));

            FileStream fileToCipher= File.OpenRead("input.txt");
            var bytes2 = new byte[1000];
            fileToCipher.Read(bytes2);
            Console.WriteLine("ciphered: '{0}'", Encoding.ASCII.GetString(bytes2));
        }
    }
}