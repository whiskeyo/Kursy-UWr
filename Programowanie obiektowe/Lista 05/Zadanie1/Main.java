// Zaimplementuj kolekcję przechowującą elementy w kolejności rosnącej wraz z metodami (lub właściwościami)
// dodania elementu, pobrania elementu (z jego usunięciem) oraz wypisania wszystkich elementów. Przyjmij,
// że przy pobieraniu elementu pobierany jest zawsze najmniejszy. Załóż, że elementy są przechowywane w tej
// kolekcji muszą implementować interfejs umożliwiający porównywanie elementów (może to być standardowy
// interfejs Comparable<T>).
// Zaimplementuj również dowolną hierarchię klas implementującą interfejs Comparable<T> lub inny zaproponowany
// przez Ciebie, zawierającą przynajmniej cztery klasy, np. hierarchia klas reprezentująca stopnie wojskowe.

// Stopnie w kolejności od najmniejszego: szeregowy, kapral, plutonowy, chorąży.
// Private -> Private First Class (PFC) -> Corporal -> Master Sergeant

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
