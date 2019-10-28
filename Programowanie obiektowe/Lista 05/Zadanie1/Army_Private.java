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
