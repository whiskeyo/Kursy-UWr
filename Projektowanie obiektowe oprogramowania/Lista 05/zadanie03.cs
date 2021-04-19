using System;
using System.Collections.Generic;

namespace Airport
{
    public class Plane { } // it does nothing, keep it simple

    interface IAirport
    {
        Plane AcquirePlane();
        void ReleasePlane(Plane plane);
    }

    public class Airport : IAirport
    {
        private int _capacity;
        private List<Plane> ready = new List<Plane>();
        private List<Plane> released = new List<Plane>();

        public Airport(int capacity)
        {
            if (capacity <= 0)
                throw new ArgumentException();

            _capacity = capacity;
        }

        public Plane AcquirePlane()
        {
            if (released.Count >= _capacity)
                throw new ArgumentException();

            if (ready.Count == 0)
                ready.Add(new Plane());

            var plane = ready[0];
            ready.Remove(plane);
            released.Add(plane);

            return plane;
        }

        public void ReleasePlane(Plane plane)
        {
            if (!released.Contains(plane))
                throw new ArgumentException();

            released.Remove(plane);
            ready.Add(plane);
        }

    }

    public class AirportProxy : IAirport
    {
        Airport airport;

        public AirportProxy(int capacity) { this.airport = new Airport(capacity); }

        public Plane AcquirePlane()
        {
            if (!IsOpen())
            {
                throw new InvalidOperationException();
            }

            return this.airport.AcquirePlane();
        }

        public void ReleasePlane(Plane plane)
        {
            if (!IsOpen())
            {
                throw new InvalidOperationException();
            }

            this.airport.ReleasePlane(plane);
        }

        private bool IsOpen()
        {
            var time = DateTime.Now;

            return (time.Hour >= 8 && time.Minute >= 0) &&
                   (time.Hour < 22 || (time.Hour == 22 && time.Minute == 0));
        }
    }
}

namespace ShapesFactory
{
    public interface IShapeFactory
    {
        void AddWorker(IShapeFactoryWorker worker);
        IShape CreateShape(string shapeName, params object[] parameters);
    }
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

    public class ShapeFactory : IShapeFactory
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

        public IShape CreateShape(string name, params object[] parameters)
        {
            foreach (var worker in _workers)
            {
                if (worker.AcceptsParameters(name, parameters))
                    return worker.Create(parameters);
            }

            throw new ArgumentException(String.Format("Unrecognised shape!"));
        }
    }

    public class ShapeFactoryProxy : IShapeFactory
    {
        ShapeFactory shapeFactory;

        public ShapeFactoryProxy() { this.shapeFactory = new ShapeFactory(); }

        public void AddWorker(IShapeFactoryWorker worker)
        {
            LogTimeOfEvent();
            Console.WriteLine("[AddWorker] AddWorker({0}) has been called.", worker);
            
            this.shapeFactory.AddWorker(worker);

            LogTimeOfEvent();
            Console.WriteLine("[AddWorker] Nothing to return.", worker);
        }

        public IShape CreateShape(string shapeName, params object[] parameters)
        {
            LogTimeOfEvent();
            Console.WriteLine("[CreateShape] CreateShape({0}, {1}) has been called.", shapeName, parameters);

            var shapeResult = this.shapeFactory.CreateShape(shapeName, parameters);

            LogTimeOfEvent();
            Console.WriteLine("[CreateShape] Return value = {0}", shapeResult);

            return shapeResult;
        }

        private void LogTimeOfEvent() { Console.Write("{0}: ", DateTime.Now); }
    }
}

namespace Exercise03
{
    class Program
    {
        static void Main(string[] args)
        {
            // AirportProxy test
            try
            {
                Airport.AirportProxy airportProxy = new Airport.AirportProxy(4);
                Airport.Plane plane1 = airportProxy.AcquirePlane();
                Airport.Plane plane2 = airportProxy.AcquirePlane();
                Airport.Plane plane3 = airportProxy.AcquirePlane();
                airportProxy.ReleasePlane(plane3);
                airportProxy.ReleasePlane(plane1);
                airportProxy.ReleasePlane(plane2);

                Console.WriteLine("Airport is working well!");
            } 
            catch (Exception)
            {
                Console.WriteLine("Airport is closed, cannot use!");
            }

            // FactoryProxy test
            ShapesFactory.ShapeFactoryProxy factoryProxy = new ShapesFactory.ShapeFactoryProxy();
            factoryProxy.AddWorker(new ShapesFactory.CircleFactoryWorker());
            factoryProxy.AddWorker(new ShapesFactory.RectangleFactoryWorker());
            factoryProxy.CreateShape("Circle", 20.1);
            factoryProxy.CreateShape("Rectangle", 1.5, 2.5);
        }
    }
}