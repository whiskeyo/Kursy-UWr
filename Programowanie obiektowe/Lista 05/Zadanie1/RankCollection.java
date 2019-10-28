import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

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
