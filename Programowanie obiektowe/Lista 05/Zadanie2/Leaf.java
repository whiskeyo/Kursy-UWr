import java.util.Hashtable;

class Variable implements Expr
{
    String const_name;

    static Hashtable<String, Integer> var_array = new Hashtable<String, Integer>();

    public Variable(String var_name)
    {
        this.const_name = var_name;
    }

    public int Eval()
    {
        return var_array.get(const_name);
    }

    public String toString()
    {
        return const_name;
    }
}

class Const implements Expr
{
    int const_value;

    public Const(int const_value)
    {
        this.const_value = const_value;
    }   

    public int Eval()
    {
        return const_value;
    }

    public String toString()
    {
        return String.valueOf(const_value);
    }
}