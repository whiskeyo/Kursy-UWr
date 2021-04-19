using System;
using System.Collections.Generic;

namespace InjectNotifier
{
    public class Person { }

    public abstract class PersonNotifier
    {
        public abstract void Notify(IEnumerable<Person> people);
    }

    public class SMSPersonNotifier : PersonNotifier
    {
        public override void Notify(IEnumerable<Person> people)
        {
            throw new NotImplementedException();
        }
    }

    public class MailPersonNotifier : PersonNotifier
    {
        public override void Notify(IEnumerable<Person> people)
        {
            throw new NotImplementedException();
        }
    }

    public abstract class PersonRegistry
    {
        PersonNotifier notifier;

        public PersonRegistry(PersonNotifier personNotifier) { this.notifier = personNotifier; }
        public abstract IEnumerable<Person> GetPeople();
        public void Notify() { notifier.Notify(GetPeople()); }
    }

    public class XMLPersonRegistry : PersonRegistry
    {
        public XMLPersonRegistry(PersonNotifier notifier) : base(notifier) { }
        public override IEnumerable<Person> GetPeople()
        {
            throw new NotImplementedException();
        }
    }

    public class DBPersonRegistry : PersonRegistry
    {
        public DBPersonRegistry(PersonNotifier notifier) : base(notifier) { }
        public override IEnumerable<Person> GetPeople()
        {
            throw new NotImplementedException();
        }
    }
}

namespace InjectRegistry
{
    public class Person { }
    
    public abstract class PersonRegistry
    {
        public abstract IEnumerable<Person> GetPeople();
    }

    public class XMLPersonRegistry : PersonRegistry
    {
        public override IEnumerable<Person> GetPeople()
        {
            throw new NotImplementedException();
        }
    }

    public class DBPersonRegistry : PersonRegistry
    {
        public override IEnumerable<Person> GetPeople()
        {
            throw new NotImplementedException();
        }
    }

    public abstract class PersonNotifier
    {
        private PersonRegistry personRegistry;

        public PersonNotifier(PersonRegistry registry) { this.personRegistry = registry; }

        public IEnumerable<Person> GetPeople() { return this.personRegistry.GetPeople(); }

        public abstract void Notify();
    }

    public class SMSPersonNotifier : PersonNotifier
    {
        public SMSPersonNotifier(PersonRegistry registry) : base(registry) { }

        public override void Notify()
        {
            throw new NotImplementedException();
        }
    }

    public class MailPersonNotifier : PersonNotifier
    {
        public MailPersonNotifier(PersonRegistry registry) : base(registry) { }

        public override void Notify()
        {
            throw new NotImplementedException();
        }
    }
}

namespace Exercise05a
{
    class Program
    {
        static void Main(string[] args)
        {

        }
    }
}