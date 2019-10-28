class Add implements Expr
{
    Expr a;
    Expr b;

    public Add(Expr ex1, Expr ex2)
    {
        a = ex1;
        b = ex2;
    }

    public int Eval()
    {
        return a.Eval() + b.Eval();
    }

    public String toString()
    {
        return "(" + a.toString() + " + " + b.toString() + ")";
    }
}

class Subtract implements Expr
{
    Expr a;
    Expr b;

    public Subtract(Expr ex1, Expr ex2)
    {
        a = ex1;
        b = ex2;
    }

    public int Eval()
    {
        return a.Eval() - b.Eval();
    }

    public String toString()
    {
        return "(" + a.toString() + " - " + b.toString() + ")";
    }
}

class Multiply implements Expr
{
    Expr a;
    Expr b;

    public Multiply(Expr ex1, Expr ex2)
    {
        a = ex1;
        b = ex2;
    }

    public int Eval()
    {
        return a.Eval() * b.Eval();
    }

    public String toString()
    {
        return "(" + a.toString() + " * " + b.toString() + ")";
    }
}

class Divide implements Expr
{
    Expr a;
    Expr b;

    public Divide(Expr ex1, Expr ex2)
    {
        a = ex1;
        b = ex2;
    }

    public int Eval() throws ArithmeticException
    {
        if (b.Eval() == 0) 
            throw new ArithmeticException("You can not divide by 0.");
            
        return a.Eval() / b.Eval();
    }

    public String toString()
    {
        return "(" + a.toString() + " / " + b.toString() + ")";
    }
}