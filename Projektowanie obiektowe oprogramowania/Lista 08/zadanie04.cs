using System;
using System.Collections.Generic;
using System.IO;

namespace Exercise04
{
    public interface IState
    {
        void InsertCard(int cardNumber);
        void ShowBalance();
        void WithdrawCash(int amount);
        void EjectCard();
    }

    public class IdleState : IState
    {
        AtmMachine machine;
        int[] currentBalances;

        public IdleState(AtmMachine machine, int[] balances) 
        {
            this.machine = machine;
            this.currentBalances = balances;
        }

        public void EjectCard()
        {
            throw new Exception("Cannot eject card without card inserted");
        }

        public void InsertCard(int cardNumber)
        {
            if (cardNumber < currentBalances.Length) // card exists in db
            {
                Console.WriteLine("Card {0} successfully inserted!", cardNumber);
                machine.SetState(new CardInsertedState(machine, currentBalances, cardNumber));
            }
            else
                throw new Exception("Card invalid, try again later or contact your provider");
        }

        public void ShowBalance()
        {
            throw new Exception("Cannot show balance without card inserted");
        }

        public void WithdrawCash(int amount)
        {
            throw new Exception("Cannot withdraw cash without card inserted");
        }
    }

    public class CardInsertedState : IState
    {
        AtmMachine machine;
        int[] currentBalances;
        int insertedCardNumber;

        public CardInsertedState(AtmMachine machine, int[] balances, int insertedCardNumber) 
        {
            this.machine = machine;
            this.currentBalances = balances;
            this.insertedCardNumber = insertedCardNumber;
        }

        public void EjectCard()
        {
            Console.WriteLine("Card successfully ejected");
            machine.SetState(new IdleState(machine, currentBalances));
        }

        public void InsertCard(int cardNumber)
        {
            throw new Exception("You can not insert another card while one is already inserted");
        }

        public void ShowBalance()
        {
            Console.WriteLine("Current balance: {0}$", currentBalances[insertedCardNumber]);
        }

        public void WithdrawCash(int amount)
        {
            currentBalances[insertedCardNumber] -= amount;
            Console.WriteLine("Withdrawed {0}$, new balance: {1}$", 
                amount, currentBalances[insertedCardNumber]);
        }
    }

    public class AtmMachine
    {
        private IState state;

        public AtmMachine(int[] balances) 
        {   
            this.state = new IdleState(this, balances);
        }

        public void SetState(IState newState) { this.state = newState; }
        public void InsertCard(int cardNumber) { this.state.InsertCard(cardNumber); }
        public void ShowBalance() { this.state.ShowBalance(); }
        public void WithdrawCash(int amount) { this.state.WithdrawCash(amount); }
        public void EjectCard() { this.state.EjectCard(); }
    }

    class Program
    {
        static void Main(string[] args)
        {
            int[] balances = { 100, 50, 1500, 160, 100, 1512, 7912 };
            var machine = new AtmMachine(balances);

            machine.InsertCard(2);
            machine.ShowBalance();
            machine.WithdrawCash(100);
            machine.WithdrawCash(300);
            machine.EjectCard();

            machine.InsertCard(0);
            machine.ShowBalance();
            machine.WithdrawCash(10);
            machine.EjectCard();
        }
    }
}