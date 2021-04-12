using System;
using System.Threading;
using NUnit.Framework;

// I came across few problems while solving this exercise, all of them were connected
// with NUnit3 and Visual Studio 2019. After downloading NUnit and NUnit3TestAdapter
// and implementing all tests I wanted to run them, but there was a strange behaviour.
// All tests were visible in the Test Explorer, but strangely after hitting "Run all
// tests in View", nothing happened. The problem is caused mainly by VS2019 (as in 2017
// it works fine) and in order to fix that, Microsoft.NET.Test.Sdk package has to be
// installed as well. Then, all tests are running perfectly fine. Also, as all tests
// are implemented in the same file as various versions of Singleton, there needs to be
// added `<GenerateProgramFile>false</GenerateProgramFile>` to `<PropertyGroup>` tag
// in .csproj file. Otherwise it does not work, as there are more than 1 main entries.

namespace Singleton
{
    public sealed class ProcessSingleton
    {
        // We need both an instance of the singleton and a lock, so other threads will
        // not create another instances when two (or more) threads try to access singleton
        // simultaneously. Constructor is private, as it is actually not needed, but there
        // is a function that can allow programmers to use the singleton's instance.
        private static ProcessSingleton instance = null;
        private static object _lock = new object();

        private ProcessSingleton() { }

        public static ProcessSingleton Instance()
        {
            if (instance == null)
            {
                lock (_lock)
                {
                    if (instance == null)
                        instance = new ProcessSingleton();
                }
            }

            return instance;
        }
    }

    public sealed class ThreadSingleton
    {
        // Here each thread can have its own static instance of a singleton. As each thread
        // should create its own copy of the singleton.
        [ThreadStatic]
        static ThreadSingleton instance;

        private ThreadSingleton() { }

        public static ThreadSingleton Instance()
        {
            if (instance == null)
                instance = new ThreadSingleton();

            return instance;
        }
    }

    public sealed class FiveSecondSingleton
    {
        // This one is very similar to the first, but it needs to take into account
        // whether the time of its existence has already expired or not. Because of
        // it, the constructor will assign a time of its creation to the lastAccessTime
        // variable.
        private static FiveSecondSingleton instance = null;
        private static object _lock = new object();

        static DateTime lastAccessTime;

        private FiveSecondSingleton() { lastAccessTime = DateTime.Now; }

        static bool HasExpired() { return DateTime.Now >= lastAccessTime.AddSeconds(5); }

        public static FiveSecondSingleton Instance()
        {
            if (instance == null || HasExpired())
            {
                lock (_lock)
                {
                    if (instance == null || HasExpired())
                        instance = new FiveSecondSingleton();
                }
            }

            return instance;
        }
    }
}

namespace Singleton.Tests
{
    [TestFixture]
    public class Tests
    {
        [Test]
        public void ProcessSingletonExists()
        {
            var singleton = Singleton.ProcessSingleton.Instance();
            Assert.NotNull(singleton);
        }

        [Test]
        public void ProcessSingleton()
        {
            // We expect to get the same instance for both vars in the whole process
            var singleton1 = Singleton.ProcessSingleton.Instance();
            var singleton2 = Singleton.ProcessSingleton.Instance();

            Assert.That(singleton1, Is.EqualTo(singleton2));
        }

        [Test]
        public void ProcessSingletonWithThreads()
        {
            // This time we expect one singleton to work on all threads
            Singleton.ProcessSingleton singleton1 = null;
            Singleton.ProcessSingleton singleton2 = null;

            var thread1 = new Thread(() => singleton1 = Singleton.ProcessSingleton.Instance());
            var thread2 = new Thread(() => singleton2 = Singleton.ProcessSingleton.Instance());

            thread1.Start();
            thread2.Start();

            thread1.Join();
            thread2.Join();

            Assert.That(singleton1, Is.EqualTo(singleton2));
        }

        [Test]
        public void ThreadSingletonExists()
        {
            var singleton = Singleton.ThreadSingleton.Instance();
            Assert.NotNull(singleton);
        }

        [Test]
        public void ThreadSingletonWithThreads()
        {
            // This time we expect each thread has its own instance of a singleton,
            // thus we want singleton1 to be different than singleton2.
            Singleton.ThreadSingleton singleton1 = null;
            Singleton.ThreadSingleton singleton2 = null;

            var thread1 = new Thread(() => singleton1 = Singleton.ThreadSingleton.Instance());
            var thread2 = new Thread(() => singleton2 = Singleton.ThreadSingleton.Instance());

            thread1.Start();
            thread2.Start();

            thread1.Join();
            thread2.Join();

            Assert.That(singleton1, Is.Not.EqualTo(singleton2));
        }

        [Test]
        public void FiveSecondSingletonExists()
        {
            var singleton = Singleton.FiveSecondSingleton.Instance();
            Assert.NotNull(singleton);
        }

        [Test]
        public void FiveSecondSingleton()
        {
            var singleton1 = Singleton.FiveSecondSingleton.Instance();
            var singleton2 = Singleton.FiveSecondSingleton.Instance();

            Assert.That(singleton1, Is.EqualTo(singleton2));
        }

        [Test]
        public void FiveSecondSingletonRestart()
        {
            var singleton1 = Singleton.FiveSecondSingleton.Instance();
            Thread.Sleep(5500);
            var singleton2 = Singleton.FiveSecondSingleton.Instance();

            Assert.That(singleton1, Is.Not.EqualTo(singleton2));
        }
    }
}

namespace Exercise01
{

    class Program
    {
        static void Main(string[] args)
        {

        }
    }
}