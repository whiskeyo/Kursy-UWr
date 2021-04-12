using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace ShapesFactory
{
    // We have an interface for shapes, similarly to the example from the lecture.
    // 'Engine' is substituted with Shape, the behaviour remains pretty similar, but
    // with this version of the factory, we also provide arguments, which are the
    // dimensions of given shapes (width, height or radius).
    // By default we register Circle and Rectangle as valid, predefined shapes. 
    // Allowing clients to create their own shapes is possible thanks to 'AddWorker'
    // function, which adds new worker to the list of existing ones. New shape, given
    // by the client is 'Square', which is implemented under rectangle and shape.
    // As mentioned in the solution to the exercise 01, in order for this program to
    // work (with tests), `<GenerateProgramFile>false</GenerateProgramFile>` needs to
    // be added, as well as package Microsoft.NET.Test.Sdk has to be installed.
    public interface IShape
    {
        double GetArea();
    }

    public class Rectangle : IShape
    {
        public double Width { get; set; }
        public double Height { get; set; }

        public double GetArea() { return Width * Height; }
    }

    public class RectangleFactoryWorker : IShapeFactoryWorker
    {
        public bool AcceptsParameters(string name, object[] parameters)
        {
            return name == "Rectangle" && parameters.Length == 2 &&
                parameters[0] is double && parameters[1] is double;
        }

        public IShape Create(object[] parameters)
        {
            return new Rectangle { Width = (double)parameters[0], Height = (double)parameters[1] };
        }
    }

    public class Circle : IShape
    {
        public double Radius { get; set; }

        public double GetArea() { return Math.PI * Radius * Radius; }
    }

    public class CircleFactoryWorker : IShapeFactoryWorker
    {
        public bool AcceptsParameters(string name, object[] parameters)
        {
            return name == "Circle" && parameters.Length == 1 && parameters[0] is double;
        }

        public IShape Create(object[] parameters)
        {
            return new Circle { Radius = (double)parameters[0] };
        }
    }

    public class Square : IShape
    {
        public double Width { get; set; }

        public double GetArea() { return Width * Width; }
    }

    public class SquareFactoryWorker : IShapeFactoryWorker
    {
        public bool AcceptsParameters(string name, object[] parameters)
        {
            return name == "Square" && parameters.Length == 1 && parameters[0] is double;
        }

        public IShape Create(object[] parameters)
        {
            return new Square { Width = (double)parameters[0] };
        }
    }

    public interface IShapeFactoryWorker 
    {
        bool AcceptsParameters(string name, object[] parameters);
        IShape Create(object[] parameters);
    }

    public class ShapeFactory
    {
        List<IShapeFactoryWorker> _workers = new List<IShapeFactoryWorker>();
        
        public ShapeFactory()
        {
            this._workers.Add(new RectangleFactoryWorker());
            this._workers.Add(new CircleFactoryWorker());
        }

        public void AddWorker(IShapeFactoryWorker worker)
        {
            _workers.Add(worker);
        }

        public IShape Create(string name, params object[] parameters)
        {
            foreach (var worker in _workers)
            {
                if (worker.AcceptsParameters(name, parameters))
                    return worker.Create(parameters);
            }

            throw new ArgumentException(String.Format("Unrecognised shape!"));
        }
    }
}

namespace ShapesFactory.Tests
{
    [TestFixture]
    public class Tests
    {
        [Test]
        public void AddSingleCircle()
        {
            ShapeFactory factory = new ShapeFactory();
            IShape circle = factory.Create("Circle", 2.0);

            Assert.IsNotNull(circle);
            Assert.That(circle.GetArea(), Is.EqualTo(Math.PI * 4));
        }

        [Test]
        public void AddSingleRectangle()
        {
            ShapeFactory factory = new ShapeFactory();
            IShape rectangle = factory.Create("Rectangle", 2.0, 11.5);

            Assert.IsNotNull(rectangle);
            Assert.That(rectangle.GetArea(), Is.EqualTo(23.0));
        }

        [Test]
        public void AddTwoShapes()
        {
            ShapeFactory factory = new ShapeFactory();
            IShape circle = factory.Create("Circle", 2.0);
            IShape rectangle = factory.Create("Rectangle", 2.0, 11.5);

            Assert.IsNotNull(circle);
            Assert.IsNotNull(rectangle);
        }

        [Test]
        public void CallWithInvalidArguments()
        {
            ShapeFactory factory = new ShapeFactory();
            Assert.Throws<ArgumentException>(
                () => factory.Create("Rectangle", "wrong", "arguments")
            );
        }

        [Test]
        public void AddNewWorkerByClient()
        {
            ShapeFactory factory = new ShapeFactory();
            factory.AddWorker(new SquareFactoryWorker());

            IShape square = factory.Create("Square", 12.0);

            Assert.IsNotNull(square);
            Assert.That(square.GetArea(), Is.EqualTo(144.0));
        }
    }
}

namespace Exercise02
{

    class Program
    {
        static void Main(string[] args)
        {

        }
    }
}