using System;
using System.Collections.Generic;
using NUnit.Framework;

namespace Airport
{
    // In this exercise we need to implement Airport class which 'deploys'
    // instances of Plane objects. The Airport has limited number of planes.
    // There needs to be a handler for exceeding the limit of the pool, as
    // well as the mechanism guaranteeing correctness of returning the object
    // to the pool. Also tests have to be made, so adding header to .csproj
    // and Microsoft.NET.Test.Sdk is required for it to work.

    public class Plane { } // it does nothing, keep it simple

    public class Airport
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
}

namespace Airport.Tests
{
    [TestFixture]
    public class Tests
    {
        [Test]
        public void CorrectCapacityAirport()
        {
            Airport airport = new Airport(10);
            Assert.NotNull(airport);
        }

        [Test]
        public void InvalidCapacityAirport()
        {
            Assert.Throws<ArgumentException>(() => new Airport(0));
            Assert.Throws<ArgumentException>(() => new Airport(-10));
        }

        [Test]
        public void ExceedAirportCapacity()
        {
            const int pool = 10;
            Airport airport = new Airport(pool);

            for (int i = 0; i < pool; i++)
            {
                Plane plane = airport.AcquirePlane();
                Assert.NotNull(plane);
            }

            Assert.Throws<ArgumentException>(() => airport.AcquirePlane());
        }

        [Test]
        public void ReleaseWrongPlane()
        {
            Airport airport = new Airport(1);
            Plane plane1 = airport.AcquirePlane();
            Plane plane2 = new Plane();

            Assert.Throws<ArgumentException>(() => airport.ReleasePlane(plane2));
        }
    }
}

namespace Exercise03
{

    class Program
    {
        static void Main(string[] args)
        {

        }
    }
}