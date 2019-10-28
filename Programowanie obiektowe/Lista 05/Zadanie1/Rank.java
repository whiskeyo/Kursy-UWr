public interface Rank extends Comparable<Rank>
{
    int get_rank();

    @Override
    int compareTo(Rank toCompare);
}
