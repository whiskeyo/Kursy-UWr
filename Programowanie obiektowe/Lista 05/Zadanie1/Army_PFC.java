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
