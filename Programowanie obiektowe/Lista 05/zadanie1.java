// Zaimplementuj kolekcję przechowującą elementy w kolejności rosnącej wraz z metodami (lub właściwościami)
// dodania elementu, pobrania elementu (z jego usunięciem) oraz wypisania wszystkich elementów. Przyjmij,
// że przy pobieraniu elementu pobierany jest zawsze najmniejszy. Załóż, że elementy są przechowywane w tej
// kolekcji muszą implementować interfejs umożliwiający porównywanie elementów (może to być standardowy
// interfejs Comparable<T>).
// Zaimplementuj również dowolną hierarchię klas implementującą interfejs Comparable<T> lub inny zaproponowany
// przez Ciebie, zawierającą przynajmniej cztery klasy, np. hierarchia klas reprezentująca stopnie wojskowe.

// Stopnie w kolejności od najmniejszego: szeregowy, kapral, plutonowy, chorąży.
// Private -> Private First Class (PFC) -> Corporal -> Master Sergeant

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main 
{
    public static void main (String[] args)
    {
        RankCollection ranks = new RankCollection();
        ranks.Add(new Army_Private());
        ranks.Add(new Army_PFC());
        ranks.Add(new Army_Corporal());
        ranks.Add(new Army_MasterSergeant());

        ranks.Print();
        System.out.println("");

        Rank some_rank = ranks.Get();
        some_rank = ranks.Get();

        System.out.println("Second picked rank is " + some_rank);
        System.out.println("");
        ranks.Print();
    }
}

public interface Rank extends Comparable<Rank>
{
    int get_rank();

    @Override
    int compareTo(Rank toCompare);
}

public class RankCollection
{
    List <Rank> rank_array; // stores elements implementing interface
    
    RankCollection()
    {
        rank_array = new ArrayList<Rank>();
    }

    void Add(Rank rank)
    {
        rank_array.add(rank);
        Collections.sort(rank_array);
    }

    Rank Get()
    {
        if (rank_array.size() == 0)
        {
            System.out.println("There are no elements.");
            return null;
        }

        Rank rank = rank_array.get(0);
        rank_array.remove(0);
        return rank;
    }

    void Print()
    {
        for (Rank rank : rank_array)
            System.out.println(rank);
    }
}

public class Army_Private implements Rank
{
    @Override
    public int get_rank()
    {
        return 10;
    }

    @Override
    public int compareTo(Rank toCompare)
    {
        return Integer.compare(this.get_rank(), toCompare.get_rank());
    }
}

public class Army_PFC implements Rank
{
    @Override
    public int get_rank()
    {
        return 20;
    }

    @Override
    public int compareTo(Rank toCompare)
    {
        return Integer.compare(this.get_rank(), toCompare.get_rank());
    }
}

public class Army_Corporal implements Rank
{
    @Override
    public int get_rank()
    {
        return 30;
    }

    @Override
    public int compareTo(Rank toCompare)
    {
        return Integer.compare(this.get_rank(), toCompare.get_rank());
    }
}

public class Army_MasterSergeant implements Rank
{
    @Override
    public int get_rank()
    {
        return 40;
    }

    @Override
    public int compareTo(Rank toCompare)
    {
        return Integer.compare(this.get_rank(), toCompare.get_rank());
    }
}