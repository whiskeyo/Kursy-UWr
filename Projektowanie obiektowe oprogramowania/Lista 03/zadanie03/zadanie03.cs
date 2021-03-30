using System;

namespace Before
{
    public class TaxCalculator
    {
        public Decimal CalculateTax(Item item)
        {
            switch (item.Category)
            {
                case "Baked goods":
                    return item.Price * 0.05m;
                case "Other":
                    return item.Price * 0.23m;
                case "Milk products":
                    return item.Price * 0.10m;
                default:
                    return item.Price;
            }
        }
    }

    public class Item
    {
        public Decimal Price { get; }
        public string Name { get; }
        public string Category { get; }

        public Item(string Name, Decimal Price, string Category)
        {
            this.Name = Name;
            this.Price = Price;
            this.Category = Category;
        }
    }

    // Classes and modules should be open for extensions but closed for modifications
    // influencing clients. The problem with this attitude is that printing bills is not
    // open for extensions, so we cannot change it. Also TaxCalculator is not open. In
    // order to fix it, we need to create interfaces for TaxTypes and for printing bills.
    // This way we might work on extensions for both taxes and bills, but we will not 
    // interrupt typical CashRegister behaviors.
    public class CashRegister
    {
        public TaxCalculator taxCalc = new TaxCalculator();

        public Decimal CalculatePrice(Item[] Items)
        {
            Decimal _price = 0;
            foreach (Item item in Items)
                _price += item.Price + taxCalc.CalculateTax(item);

            return _price;
        }

        public string PrintBill(Item[] Items)
        {
            Decimal _totalPrice = 0m;
            Decimal _totalTax = 0m;
            string Bill = "";

            foreach (var item in Items)
            {
                Bill += "Product : " + item.Name + " : price " + item.Price
                    + " + tax " + taxCalc.CalculateTax(item).ToString("0.##") + "\n";

                _totalPrice += item.Price;
                _totalTax += taxCalc.CalculateTax(item);
            }

            Bill += "Total price:  " + (_totalPrice + _totalTax).ToString("0.##PLN\n");
            Bill += "For products: " + _totalPrice.ToString("0.##PLN\n");
            Bill += "Tax:          " + _totalTax.ToString("0.##PLN\n");

            return Bill;
        }
    }
}

namespace After
{
    public interface ITaxType
    {
        public Decimal Tax();
    }

    public class TaxBakedGoods : ITaxType
    {
        public Decimal Tax()
        {
            return 0.05m;
        }
    }

    public class TaxMilkProducts : ITaxType
    {
        public Decimal Tax()
        {
            return 0.10m;
        }
    }

    public class TaxOther : ITaxType
    {
        public Decimal Tax()
        {
            return 0.23m;
        }
    }

    public class TaxCalculator
    {
        public Decimal CalculateTax(Item item)
        {
            return item.Price * item.TaxType.Tax();
        }
    }

    public class Item
    {
        public Decimal Price { get; }
        public string Name { get; }
        public ITaxType TaxType { get; }

        public Item(string Name, Decimal Price, ITaxType TaxType)
        {
            this.Name = Name;
            this.Price = Price;
            this.TaxType = TaxType;
        }
    }

    public interface IBillPrinter
    {
        public string PrintBill(Item[] Items);
    }

    public class AlphabeticalBillPrinter : IBillPrinter
    {
        public string PrintBill(Item[] Items)
        {
            Array.Sort(Items, delegate (Item x, Item y) { return x.Name.CompareTo(y.Name); });

            Decimal _totalPrice = 0m;
            Decimal _totalTax = 0m;
            string Bill = "";

            foreach (var item in Items)
            {
                Bill += "Product : " + item.Name + " : price " + item.Price
                    + " + tax " + (item.Price * item.TaxType.Tax()).ToString("0.##") + "\n";

                _totalPrice += item.Price;
                _totalTax += item.Price * item.TaxType.Tax();
            }

            Bill += "Total price:  " + (_totalPrice + _totalTax).ToString("0.##PLN\n");
            Bill += "For products: " + _totalPrice.ToString("0.##PLN\n");
            Bill += "Tax:          " + _totalTax.ToString("0.##PLN\n");

            return Bill;
        }
    }

    public class NormalBillPrinter : IBillPrinter
    {
        public string PrintBill(Item[] Items)
        {
            Decimal _totalPrice = 0m;
            Decimal _totalTax = 0m;
            string Bill = "";

            foreach (var item in Items)
            {
                Bill += "Product : " + item.Name + " : price " + item.Price
                    + " + tax " + (item.Price * item.TaxType.Tax()).ToString("0.##") + "\n";

                _totalPrice += item.Price;
                _totalTax += item.Price * item.TaxType.Tax();
            }

            Bill += "Total price:  " + (_totalPrice + _totalTax).ToString("0.##PLN\n");
            Bill += "For products: " + _totalPrice.ToString("0.##PLN\n");
            Bill += "Tax:          " + _totalTax.ToString("0.##PLN\n");

            return Bill;
        }
    }

    public class CashRegister
    {
        public TaxCalculator taxCalc = new TaxCalculator();
        public IBillPrinter billPrinter;

        public CashRegister(IBillPrinter billPrinter) { this.billPrinter = billPrinter; }

        public Decimal CalculatePrice(Item[] Items)
        {
            Decimal _price = 0;
            foreach (Item item in Items)
                _price += item.Price + taxCalc.CalculateTax(item);

            return _price;
        }

        public string PrintBill(Item[] items)
        {
            return billPrinter.PrintBill(items);
        }
    }
}

namespace Exercise03
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Code which does not respect OCP");
            Before.Item[] beforeItems = new Before.Item[5];
            beforeItems[0] = new Before.Item("Bread", 3.49m, "Baked goods");
            beforeItems[1] = new Before.Item("Panini", 4.49m, "Baked goods");
            beforeItems[2] = new Before.Item("Tiger", 3.99m, "Other");
            beforeItems[3] = new Before.Item("Cheese", 2.49m, "Milk products");
            beforeItems[4] = new Before.Item("Milk", 1.79m, "Milk products");

            Before.CashRegister cashRegister = new Before.CashRegister();
            Console.WriteLine(cashRegister.PrintBill(beforeItems));

            Console.WriteLine("\n\nCode which respects OCP");
            After.TaxBakedGoods bakedGoods = new After.TaxBakedGoods();
            After.TaxMilkProducts milkProducts = new After.TaxMilkProducts();
            After.TaxOther other = new After.TaxOther();

            After.AlphabeticalBillPrinter alphabeticalBillPrinter = new After.AlphabeticalBillPrinter();
            After.NormalBillPrinter normalBillPrinter = new After.NormalBillPrinter();

            After.Item[] afterItems = new After.Item[5];
            afterItems[0] = new After.Item("Bread", 3.49m, bakedGoods);
            afterItems[1] = new After.Item("Panini", 4.49m, bakedGoods);
            afterItems[2] = new After.Item("Tiger", 3.99m, other);
            afterItems[3] = new After.Item("Cheese", 2.49m, milkProducts);
            afterItems[4] = new After.Item("Milk", 1.79m, milkProducts);

            After.CashRegister cashRegisterAlphabetical = new After.CashRegister(alphabeticalBillPrinter);
            After.CashRegister cashRegisterNormal = new After.CashRegister(normalBillPrinter);

            Console.WriteLine("NORMAL BILL");
            Console.WriteLine(cashRegisterNormal.PrintBill(afterItems));
            Console.WriteLine("ALPHABETICALLY SORTED BILL");
            Console.WriteLine(cashRegisterAlphabetical.PrintBill(afterItems));
        }
    }
}