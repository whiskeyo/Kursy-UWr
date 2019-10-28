import java.io.Serializable;

public class Vehicle implements Serializable
{
    int year;
    String name;
    double mileage;

    Vehicle()
    {
        year = 0;
        mileage = 0.0;
        name = "unknown name";
    }

    Vehicle (int year, String name, double mileage)
    {
        this.year = year;
        this.name = name;
        this.mileage = mileage;
    }

    public String toString()
    {
        return name + ", year: " + year + "\n";
    }
}