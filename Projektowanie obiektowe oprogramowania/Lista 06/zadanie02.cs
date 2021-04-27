using System;
using System.Collections.Generic;

namespace Exercise02
{
    public class Context
    {
        private Dictionary<string, bool> variables = new Dictionary<string, bool>();

        public bool GetValue(string VariableName)
        {
            if (variables.ContainsKey(VariableName))
                return variables[VariableName];

            else
                throw new ArgumentException("Variable '{0}' is undefined", VariableName);
        }

        public void setValue(string VariableName, bool Value)
        {
            if (variables.ContainsKey(VariableName))
                variables.Remove(VariableName);

            variables.Add(VariableName, Value);
        }
    }

    public abstract class AbstractExpression
    {
        public abstract bool Interpret(Context context);
    }

    public class ConstExpression : AbstractExpression
    {
        public string variableName;

        public ConstExpression(string variableName) { this.variableName = variableName; }

        public override bool Interpret(Context context)
        {
            return context.GetValue(variableName);
        }
    }

    public class BinaryExpression : AbstractExpression 
    {
        public AbstractExpression LHS;
        public AbstractExpression RHS;
        public string Operator;

        public override bool Interpret(Context context)
        {
            switch (Operator)
            {
                case "|":
                case "||":
                    return this.LHS.Interpret(context) || this.RHS.Interpret(context);
                case "&":
                case "&&":
                    return this.LHS.Interpret(context) && this.RHS.Interpret(context);
                default:
                    throw new ArgumentException("Operator unknown");
            }
        }
    }

    public class UnaryExpression : AbstractExpression 
    {
        public AbstractExpression Expression;
        public string Operator;

        public override bool Interpret(Context context)
        {
            switch (Operator)
            {
                case "":
                    return this.Expression.Interpret(context);
                case "!":
                    return !this.Expression.Interpret(context);
                default:
                    throw new ArgumentException("Operator unknown");
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Expression: !(p OR q) AND r");

            AbstractExpression expression = new BinaryExpression()
            {
                Operator = "&&",
                LHS = new UnaryExpression()
                {
                    Operator = "!",
                    Expression = new BinaryExpression()
                    {
                        Operator = "||",
                        LHS = new ConstExpression("p"),
                        RHS = new ConstExpression("q")
                    }
                },
                RHS = new ConstExpression("r")
            };


            Console.WriteLine("Truth table for given expression:");
            Console.WriteLine("p q r E");
            Context context = new Context();
            for (var a = 0; a < 2; a++)
                for (var b = 0; b < 2; b++)
                    for (var c = 0; c < 2; c++)
                    {
                        context.setValue("p", Convert.ToBoolean(a));
                        context.setValue("q", Convert.ToBoolean(b));
                        context.setValue("r", Convert.ToBoolean(c));
                        Console.WriteLine("{0} {1} {2} {3}", a, b, c, expression.Interpret(context));
                    }
        }
    }
}