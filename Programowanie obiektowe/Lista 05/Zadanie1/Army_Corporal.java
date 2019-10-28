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
