using System;

// Rectangle has postconditions:
//      Rectangle(width)  = width 
//      Rectangle(height) = height
// Square has postconditions:
//      Rectangle(width)  = width OR height
//      Rectangle(height) = height OR width
// In order to keep LSP, order of inheritance needs to be changed, as Rectangle
// has stronger postconditions than Square.

namespace Exercise04
{
    public class Square
    {
        protected int Width;

        public int GetWidth()
        {
            return Width;
        }

        public virtual void SetWidth(int value)
        {
            Width = value;
            SetHeight(value);
        }

        protected int Height;

        public int GetHeight()
        {
            return Height;
        }

        public virtual void SetHeight(int value)
        {
            Height = value;
            Width = value;
        }

        public Square (int Width, int Height)
        {
            this.SetWidth(Width);
            this.SetHeight(Height);
        }
    }

    public class Rectangle : Square
    {
        public override void SetWidth(int value) { this.Width = value; }
        public override void SetHeight(int value) { this.Height = value; }

        public Rectangle(int Width, int Height) : base(Width, Height) { }
    }

    public class AreaCalculator
    {
        public int CalculateArea(Square square)
        {
            return square.GetHeight() * square.GetWidth();
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            int w = 10; int h = 15;
            Square rect = new Rectangle(w, h);
            AreaCalculator calculator = new AreaCalculator();

            Console.WriteLine("Area of {0}x{1} rectangle is {2}", 
                w, h, calculator.CalculateArea(rect));
        }
    }
}