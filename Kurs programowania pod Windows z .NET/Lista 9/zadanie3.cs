using System;
using System.Xml;
using System.Xml.Schema;
using System.Xml.XPath;

namespace XPathValidation
{
    class Program
    {
        static void ValidationEventHandler(object sender, ValidationEventArgs e)
        {
            switch (e.Severity)
            {
                case XmlSeverityType.Error:
                    Console.WriteLine("Error: {0}", e.Message);
                    break;
                case XmlSeverityType.Warning:
                    Console.WriteLine("Warning: {0}", e.Message);
                    break;
            }
        }
        static void Main(string[] args)
        {
            try
            {
                XmlReaderSettings settings = new XmlReaderSettings();
                settings.Schemas.Add("mojedane", "zadanie2.xsd");
                settings.ValidationType = ValidationType.Schema;

                XmlReader reader = XmlReader.Create("zadanie1.xml", settings);
                XmlDocument document = new XmlDocument();
                document.Load(reader);

                ValidationEventHandler eventHandler = new ValidationEventHandler(ValidationEventHandler);
                document.Validate(eventHandler);

                // aby weryfikacja przeszla pomyslnie, nalezy skomentowac ponizsze linijki
                XPathNavigator navigator = document.CreateNavigator();
                navigator.MoveToFollowing("Imie", "mojedane");
                XmlWriter writer = navigator.InsertAfter();
                writer.WriteStartElement("cosspozatabeli", "mojedane");
                writer.WriteEndElement();
                writer.Close();

                document.Validate(eventHandler);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.WriteLine("Koniec weryfikacji.");
            Console.ReadKey();
        }
    }
}
