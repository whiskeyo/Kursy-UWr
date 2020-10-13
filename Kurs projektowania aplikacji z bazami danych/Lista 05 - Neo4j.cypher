//MATCH (n:Ksiazka) DELETE n
CREATE (:Ksiazka{
    ISBN:           '83-246-0279-8',
    Tytul:          'Microsoft Access. Podrecznik administratora',
    Autor:          'Helen Feddema',
    Rok_Wydania:    2006,
    Cena:           69
})

CREATE (:Ksiazka{
    ISBN:           '83-246-0653-X',
    Tytul:          'SQL Server 2005. Programowanie. Od podstaw',
    Autor:          'Robert Vieira',
    Rok_Wydania:    2007,
    Cena:           97
})

CREATE (:Egzemplarz{ Sygnatura: 'S0001' })
CREATE (:Egzemplarz{ Sygnatura: 'S0002' })
CREATE (:Egzemplarz{ Sygnatura: 'S0003' })

CREATE (:Czytelnik{
    PESEL:                  '55101011111',
    Nazwisko:               'Kowalski',
    Miasto:                 'Wroclaw',
    DataUrodzenia:          date('1955-10-10'),
    OstatnieWypozyczenie:   date('2020-02-01')
})

CREATE (:Czytelnik{
    PESEL:                  '60101033333',
    Nazwisko:               'Maliniak',
    Miasto:                 'Wroclaw',
    DataUrodzenia:          date('1960-10-10'),
    OstatnieWypozyczenie:   date('2020-03-01')
})

MATCH (a:Ksiazka {Tytul: 'Microsoft Access. Podrecznik administratora'})
MATCH (b:Egzemplarz {Sygnatura: 'S0001'})
MERGE (a)-[:Zawiera]->(b)

MATCH (a:Ksiazka {Tytul: 'SQL Server 2005. Programowanie. Od podstaw'})
MATCH (b:Egzemplarz {Sygnatura: 'S0002'})
MERGE (a)-[:Zawiera]->(b)

MATCH (a:Ksiazka {Tytul: 'SQL Server 2005. Programowanie. Od podstaw'})
MATCH (b:Egzemplarz {Sygnatura: 'S0003'})
MERGE (a)-[:Zawiera]->(b)

// MATCH
// (:Ksiazka) -[r:Zawiera]-> (:Egzemplarz)
// DELETE r

MATCH (a:Czytelnik {PESEL: '55101011111'})
MATCH (b:Egzemplarz {Sygnatura: 'S0001'})
MERGE (a)-[:Wypozyczyl {Data: date('2019-12-01'), Liczba_Dni: 30}]->(b)

MATCH (a:Czytelnik {PESEL: '55101011111'})
MATCH (b:Egzemplarz {Sygnatura: 'S0002'})
MERGE (a)-[:Wypozyczyl {Data: date('2020-02-01'), Liczba_Dni: 30}]->(b)

MATCH (a:Czytelnik {PESEL: '60101033333'})
MATCH (b:Egzemplarz {Sygnatura: 'S0001'})
MERGE (a)-[:Wypozyczyl {Data: date('2020-03-01'), Liczba_Dni: 60}]->(b)

MATCH (a:Czytelnik {PESEL: '60101033333'})
MATCH (b:Egzemplarz {Sygnatura: 'S0003'})
MERGE (a)-[:Wypozyczyl {Data: date('2020-01-15'), Liczba_Dni: 30}]->(b)

// Pokazanie wszystkich relacji między danymi
MATCH (n)-[r]->(m) RETURN n, r, m

CREATE CONSTRAINT peselC
ON (n:Czytelnik)
ASSERT n.PESEL IS UNIQUE

CREATE CONSTRAINT sygnaturaC
ON (n:Egzemplarz)
ASSERT n.Sygnatura IS UNIQUE

// Znajdź czytelnika, który wypożyczył chociaż jedną książkę na 30 dni
MATCH (a:Czytelnik)-[r:Wypozyczyl]->(b:Egzemplarz)
WHERE r.Liczba_Dni=30
RETURN DISTINCT a.Nazwisko

// Znajdź czytelników, którzy urodzili się przed 1960 rokiem
MATCH (a:Czytelnik)
WHERE a.DataUrodzenia < date('1960-1-1')
RETURN a

// Znajdź ile jest egzemplarzy danej ksiazki
MATCH (a:Ksiazka)-[r:Zawiera]->(b:Egzemplarz)
RETURN a.Tytul, COUNT(r)

// Znajdź ile osób wypożyczyło daną książkę
MATCH (a:Czytelnik) - [:Wypozyczyl] -> (:Egzemplarz) <- [:Zawiera] - (b:Ksiazka)
RETURN b.Tytul, COUNT(DISTINCT a)

// Zmien cenę książki
MATCH (n:Ksiazka {Tytul:'Microsoft Access. Podrecznik administratora'})
set n.Cena = 59

// Dodaj egzemplarz ksiazki. MERGE to MATCH-or-CREATE
MATCH (a:Ksiazka {Tytul: 'Microsoft Access. Podrecznik administratora'})
MERGE (a)-[:Zawiera]->(:Egzemplarz {Sygnatura: 'S0004'})

// Cofnij te zmianę
MATCH (:Ksiazka {Tytul: 'Microsoft Access. Podrecznik administratora'})-[r:Zawiera]->(n:Egzemplarz {Sygnatura: 'S0004'})
DELETE r, n

// Usun czytelnika, DETACH usuwa także wszystkie relacje
MATCH(n:Czytelnik {PESEL: "60101033333"})
DETACH DELETE n