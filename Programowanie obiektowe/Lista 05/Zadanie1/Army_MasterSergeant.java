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
