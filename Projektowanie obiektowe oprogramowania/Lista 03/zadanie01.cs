using System;
using System.Diagnostics;

namespace Exercise01
{
    // GRASP principles:
    // * Creator
    // * Information Expert
    // * Controller
    // * Low Coupling
    // * High Cohesion
    // * Polymorphism
    // * Indirection
    // * Pure fabrication
    // * Protected Variations

    // Shape, Circle and Square classes are the example of polymorphism. Both Circle and Square
    // are two-dimensional shapes, which means that they have the area, which is derived from
    // the base class Shape.
    public class Shape
    {
        public virtual double Area() { return 0; }
    }

    public class Circle : Shape
    {
        public double radius { get; set; }
        public Circle() { radius = 5; }
        public override double Area()
        {
            return Math.PI * Math.Pow(radius, 2);
        }
    }

    public class Square : Shape
    {
        public double length { get; set; }
        public Square() { length = 5; }
        public override double Area()
        {
            return Math.Pow(length, 2);
        }
    }

    // Below there are few GRASP principles shown. We want to create multiple classes which
    // are similar and then make some tests checking their correctness. 
    // * Interface connects them all, which means Low Coupling principle is present. 
    // * Also all of Test classes are meant to do just one thing - make test. It means that
    //   High Cohesion principle is complete, as each class has just one thing to do.
    // * TestController is the main class - it has all needed information to run tests based
    //   on the name of command (string). Moreover it is the class responsible for returning
    //   tests output, so it has to make the test - that means it has to create the test as
    //   well. It means that Information Expert, Protected Variations and Creator principles
    //   are present in this example.

    class TestController
    {
        public void MakeTest(string testName, long testsQuantity)
        {
            ITest test;
            long time;

            switch (testName)
            {
                case "AddIntTest":
                    test = new AddIntTest();
                    time = MeasureTime(test, testsQuantity);
                    Console.WriteLine("Test '" + testName + "' completed in " + time + " ms.");
                    break;
                case "AddInt32Test":
                    test = new AddInt32Test();
                    time = MeasureTime(test, testsQuantity);
                    Console.WriteLine("Test '" + testName + "' completed in " + time + " ms.");
                    break;
                case "MultiplyDoubleTest":
                    test = new MultiplyDoubleTest();
                    time = MeasureTime(test, testsQuantity);
                    Console.WriteLine("Test '" + testName + "' completed in " + time + " ms.");
                    break;
            }
        }

        public long MeasureTime(ITest test, long testsQuantity)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            test.MakeTest(testsQuantity);
            sw.Stop();
            return sw.ElapsedMilliseconds;
        }
    }

    interface ITest
    {
        public void MakeTest(long testsQuantity);
    }

    class AddIntTest : ITest
    {
        private int a = 10;
        private int b = 1;

        public void MakeTest(long testsQuantity)
        {
            for (long i = 0; i < testsQuantity; i++)
                a += b;
        }
    }
    class AddInt32Test : ITest
    {
        private Int32 a = 10;
        private Int32 b = 1;

        public void MakeTest(long testsQuantity)
        {
            for (long i = 0; i < testsQuantity; i++)
                a += b;
        }
    }

    class MultiplyDoubleTest : ITest
    {
        private double a = 3.5;
        private double b = 1.23;

        public void MakeTest(long testsQuantity)
        {
            for (long i = 0; i < testsQuantity; i++)
                a *= b;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            // Example of polymorhpism principle usage:
            Console.WriteLine("Polymorphism principle - Circle and Square derive from the class Shape:");
            Shape circle = new Circle();
            Shape square = new Square();
            Console.WriteLine("Area of circle: " + circle.Area());
            Console.WriteLine("Area of square: " + square.Area());

            Console.WriteLine("\n\nOther principles presented with tests:");
            TestController testController = new TestController();
            testController.MakeTest("AddIntTest", 1000000000);
            testController.MakeTest("AddInt32Test", 1000000000);
            testController.MakeTest("MultiplyDoubleTest", 1000000000);
        }
    }
}
