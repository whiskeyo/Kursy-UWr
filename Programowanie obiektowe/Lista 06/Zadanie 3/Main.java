// Zaprogramuj klasę implementującą dostęp do bufora o stałym rozmiarze przechowującym elementy typu generycznego T.
// Rozmiar bufora jest zadawany w konstruktorze. Implementacja powinna umożliwić działanie takiego bufora
// w środowisku wielowątkowym. Kolejność elementów pobieranych z bufora powinna być taka sama jak kolejność ich
// wkładania do bufora. Korzystając z tej klasy zaimplementuj problem producenta-konsumenta: producent produkuje 
// wyniki (napisy) i wkłada je do bufora. Jeśli bufor jest pełny, to producent zasypia czekając aż zwolni się 
// miejsce w buforze. Konsument, jeśli w buforze jest jakiś element (napis), to go pobiera i "konsumuje".
// Zaimplementuj producenta i konsumenta jako dwa odrębne wątki. Przed implementacją tego zadania warto zapoznać się
// z metodami wait() i notify() klasy Thread.

public class Main
{
    public static void main(String[] args)
    {
        Buffer<String> buffer = new Buffer<String>(20);

        // These two are different threads, as they extend Thread class and implement their own run()
        // method in order to insert or save strings.
        Producer producer = new Producer(buffer);
        Consumer consumer = new Consumer(buffer);

        // Now methods run() are executed, they use buffer class by turns, but only one method can be
        // used at a time (guaranteed by 'synchronized' in Buffer). Methods wait until it is their turn.
        producer.start();
        consumer.start();
    }
}