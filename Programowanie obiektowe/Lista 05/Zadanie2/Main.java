// Wyrażenia arytmetyczne można reprezentować jako drzewa, gdzie w liściach pamiętane są liczby, a w węzłach
// symbole operacji arytmetycznych. Zaimplementuj w Javie odpowiednie klasy reprezentujące węzły i liście
// takiego drzewa jako podklasy klasy Wyrazenie (Expr). W każdej klasie zdefiniuj metodę public int Eval();
// obliczającą wartość wyrażenia reprezentowanego przez obiekt. Zdefiniuj odpowiednie konstruktory. Przyjmij,
// że w liściach mogą być zarówno stałe liczbowe jak i zmienne. Przyjmij, że wartości zmiennych są przechowywane
// np. w tablicy haszującej. Zaprogramuj w każdej klasie metodę toString() zwracającą wyrażenie w postaci napisu.

import java.util.Hashtable;

public class Main
{
    public static void main(String[] args)
    {
        Hashtable<String, Integer> varArray = new Hashtable<String, Integer>();

        varArray.put("x", 1);
        varArray.put("z", 2);
        varArray.put("y", 3);

        System.out.println("x = " + varArray.get("x"));
        System.out.println("y = " + varArray.get("y"));
        System.out.println("z = " + varArray.get("z"));

        Variable.var_array = varArray; // we set the array of variables to evaluate

        // the first Expr is used for the next ones as the base
        // (((6 * (2 + x)) / y) - 4) = 5
        Expr test1 = new Add(new Const(2), new Variable("x"));
        test1 = new Multiply(new Const(6), test1);
        test1 = new Divide(test1, new Variable("z"));
        test1 = new Subtract(test1, new Const(1));
        System.out.println(test1.toString() + " = " + test1.Eval());

        // ((y + x) * z) = 9
        Expr test2 = new Add(new Variable("y"), new Variable("x"));
        test2 = new Multiply(test2, new Variable("z"));
        System.out.println(test2.toString() + " = " + test2.Eval());

        // Should throw an exception "Cannot divide by 0."
        Expr test3 = new Divide(new Const(0), new Const(4));
        test3 = new Divide(test3, new Const(0));
        System.out.println(test3.toString() + " = " + test3.Eval());
    }
}