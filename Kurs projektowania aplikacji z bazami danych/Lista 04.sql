-- Zadanie 1 --
-- duzo info: https://sqlchris.wordpress.com/2017/06/25/poziomy-izolacji-transakcji-sql-server/

-- USTAWIENIE POZIOMU IZOLACJI DO TESTÓW
-- SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; 
-- SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
-- SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
-- SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;

-- POZIOM IZOLACJI		DIRTY READ		NONREAPEATABLE READ		PHANTOM
-- READ UNCOMMITTED		TAK				TAK						TAK
-- READ COMMITTED		NIE				TAK						TAK
-- REPEATABLE READ		NIE				NIE						TAK
-- SNAPSHOT				NIE				NIE						NIE
-- SERIALIZABLE			NIE				NIE						NIE

-- Odczyt brudnych danych --
DROP TABLE IF EXISTS Magazyn
GO

CREATE TABLE Magazyn (ID INT PRIMARY KEY, Produkt VARCHAR(50), LiczbaSztuk INT)
INSERT INTO Magazyn VALUES(1, 'pi³ka',		 13)
INSERT INTO Magazyn VALUES(2, 'kij',		 3)
INSERT INTO Magazyn VALUES(3, 'bramka',		 4)
INSERT INTO Magazyn VALUES(4, 'rower',		 6)
INSERT INTO Magazyn VALUES(5, 'ochraniacze', 1)
GO

/*
Sposób dzia³ania: odpalamy w dwóch okienkach SSMS, pierwsz¹ transakcjê wykonujemy normalnie, po czym
jednoczeœnie w³¹czamy drug¹. Oczekiwanym wynikiem by³oby wyœwietlenie 12 pi³ek i 3 rowerów w drugiej
transakcji, gdy¿ zosta³a ona wykonana jednoczeœnie z pierwsz¹, jednak przez to, ¿e musimy czekaæ do
momentu wykonania pierwszej transakcji (i rollbacku), drugie zapytanie wyœwietli niezmienione dane.
*/

-- Transakcja 1 --

BEGIN TRAN
UPDATE Magazyn SET LiczbaSztuk = 12 WHERE ID = 1
UPDATE Magazyn SET LiczbaSztuk = 3  WHERE ID = 4
-- czekamy na przyjêcie pieniêdzy od klienta, st¹d delay
WAITFOR DELAY '00:00:05'
SELECT * FROM Magazyn -- dane zmieniaja sie, ale rollback je cofa
ROLLBACK TRANSACTION
SELECT * FROM Magazyn

-- Transakcja 2 --
SELECT * FROM Magazyn

-- Niepowtarzaloœæ odczytów --
DROP TABLE IF EXISTS Ludzie 
GO

CREATE TABLE Ludzie (Imie VARCHAR(20), Nazwisko VARCHAR(20), Wiek INT)
INSERT INTO Ludzie VALUES('Tomasz', 'G¹bka',    15)
INSERT INTO Ludzie VALUES('Arnold', 'Mas³o',    33)
INSERT INTO Ludzie VALUES('Arek',   'Nowak',    23)
INSERT INTO Ludzie VALUES('Micha³', 'Kowalski', 13)
INSERT INTO Ludzie VALUES('Oskar',  'Nowacki',  20)
INSERT INTO Ludzie VALUES('Luke',   'Smith',    22)
GO

/*
Sposób dzia³ania jak przy odczycie brudnych danych, tutaj dochodzi do sytuacji, w której dane s¹
zmieniane w drugiej transakcji, przez co dwa odczyty danych w pierwszej transakcji zwracaj¹ inne
wyniki.
*/

-- Transakcja 1 --
BEGIN TRAN
SELECT * FROM Ludzie
WAITFOR DELAY '00:00:05'
SELECT * FROM Ludzie
ROLLBACK
GO

-- Transakcja 2 --
BEGIN TRAN
UPDATE Ludzie SET Wiek = 50 WHERE Imie = 'Tomasz' OR Imie = 'Oskar'
COMMIT
GO

-- Odczyty fantomów --
DROP TABLE IF EXISTS Samochody
GO

CREATE TABLE Samochody (Model VARCHAR(20), Marka VARCHAR(20))
INSERT INTO Samochody VALUES('Opel',  'Astra')
INSERT INTO Samochody VALUES('Opel',  'Meriva')
INSERT INTO Samochody VALUES('Skoda', 'Fabia')
INSERT INTO Samochody VALUES('Skoda', 'Superb')
INSERT INTO Samochody VALUES('Skoda', 'Felicia')
GO

/*
Sposób dzia³ania jak wy¿ej, jednak przy odczytach zmienia siê liczba odczytywanych danych
spowodowana dzia³aniem INSERT lub DELETE.
*/

-- Transakcja 1 --
BEGIN TRAN
SELECT * FROM Samochody
WAITFOR DELAY '00:00:05'
SELECT * FROM Samochody
ROLLBACK

-- Transakcja 2 --
BEGIN TRAN
DELETE FROM Samochody WHERE Model = 'Opel'
COMMIT
GO


-- Zadanie 2 --
-- Tabelka g³ówna z zadania do normalizacji --
DROP TABLE IF EXISTS Wizyty
GO

CREATE TABLE Wizyty(
	[ID]					INT PRIMARY KEY,
	[Pacjent]				VARCHAR(50),
	[Adres Pacjenta]		VARCHAR(100),
	[Data i miejsce wizyty] VARCHAR(100),
	[Kwota]					MONEY,
	[Lekarz]				VARCHAR(50),
	[Powód wizyty]			VARCHAR(100))

INSERT INTO Wizyty VALUES
(1, 'Jan Kot',    'ul. Dolna 6, 44-444 Bór', '2029-02-01 12:30, pok. 12', 300, 'Oleg Wyrwiz¹b', 'Dentystyczny: za³o¿enie protezy w (...)'),
(2, 'Maria Mysz', 'ul. Górna 9, 55-555 Las', '2030-01-04 11:45, pok. 7',  150, 'Ewa Ciarka',    'Dermatologiczny: oglêdziny znamiona (...)')
GO

SELECT * FROM Wizyty
GO

-- Tabelka w 1NF --
DROP TABLE IF EXISTS Wizyty1NF
GO

CREATE TABLE Wizyty1NF(
	[ID]					INT PRIMARY KEY,
	[Pacjent]				VARCHAR(50),
	[Adres]					VARCHAR(50),
	[Kod pocztowy]			VARCHAR(10),
	[Miasto]				VARCHAR(30),
	[Data wizyty]			SMALLDATETIME,
	[Miejsce wizyty (pok)]	INT,
	[Kwota]					MONEY,
	[Lekarz]				VARCHAR(50),
	[Typ wizyty]			VARCHAR(30),
	[Powód wizyty]			VARCHAR(100))

INSERT INTO Wizyty1NF VALUES
(1, 'Jan Kot',	  'ul. Dolna 6', '44-444', 'Bór', '2029-02-01 12:30', 12, 300, 'Oleg Wyrwiz¹b', 'Dentystyczny',    'Za³o¿enie protezy w (...)'),
(2, 'Maria Mysz', 'ul. Górna 9', '55-555', 'Las', '2030-01-04 11:45', 7,  150, 'Ewa Ciarka',	'Dermatologiczny', 'Oglêdziny znamiona (...)')
GO

SELECT * FROM Wizyty1NF
GO

-- Tabelki w 2NF (³¹czymy dane kluczem podstawowym, w tym wypadku jest nim ID pacjenta) --
DROP TABLE IF EXISTS Pacjenci
GO

CREATE TABLE Pacjenci(
	[ID]					INT PRIMARY KEY,
	[Pacjent]				VARCHAR(50),
	[Adres]					VARCHAR(50),
	[Kod pocztowy]			VARCHAR(10),
	[Miasto]				VARCHAR(30))

INSERT INTO Pacjenci VALUES
(1, 'Jan Kot',	  'ul. Dolna 6', '44-444', 'Bór'),
(2, 'Maria Mysz', 'ul. Górna 9', '55-555', 'Las')
GO

SELECT * FROM Pacjenci
GO

DROP TABLE IF EXISTS Wizyty2NF	
GO

CREATE TABLE Wizyty2NF(
	[ID Pacjenta]			INT,
	[Data wizyty]			SMALLDATETIME,
	[Miejsce wizyty (pok)]	INT,
	[Kwota]					MONEY,
	[Lekarz]				VARCHAR(50),
	[Typ wizyty]			VARCHAR(30),
	[Powód wizyty]			VARCHAR(100))

INSERT INTO Wizyty2NF VALUES
(1, '2029-02-01 12:30', 12, 300, 'Oleg Wyrwiz¹b', 'Dentystyczny',    'Za³o¿enie protezy w (...)'),
(2, '2030-01-04 11:45', 7,  150, 'Ewa Ciarka',	  'Dermatologiczny', 'Oglêdziny znamiona (...)')
GO

SELECT * FROM Wizyty2NF
GO

SELECT * FROM Pacjenci JOIN Wizyty2NF ON Pacjenci.ID = Wizyty2NF.[ID Pacjenta]
GO

-- Tabelki 3NF (pozbycie sie danych, ktore mozemy wywnioskowac za pomoca innych), tabelka Pacjenci zostaje --
-- Je¿eli lekarz ma swój pokój, to te¿ siê tego pozbywamy
DROP TABLE IF EXISTS Wizyty3NF
GO

CREATE TABLE Wizyty3NF(
	[ID Pacjenta]			INT,
	[Data wizyty]			SMALLDATETIME,
	[Kwota]					MONEY,
	[Lekarz]				VARCHAR(50),
	[Powód wizyty]			VARCHAR(100))

INSERT INTO Wizyty3NF VALUES
(1, '2029-02-01 12:30', 300, 'Oleg Wyrwiz¹b', 'Za³o¿enie protezy w (...)'),
(2, '2030-01-04 11:45', 150, 'Ewa Ciarka',	  'Oglêdziny znamiona (...)')
GO

SELECT * FROM Wizyty3NF
GO

SELECT * FROM Pacjenci JOIN Wizyty3NF ON Pacjenci.ID = Wizyty3NF.[ID Pacjenta]
GO

-- Zadanie 3 --
-- Dane zaczerpniête z pliku biblioteka.sql z przyk³adów podstaw T-SQL:
DROP TABLE IF EXISTS Wypozyczenie;
GO

DROP TABLE IF EXISTS Egzemplarz;
GO

DROP TABLE IF EXISTS Czytelnik;
GO

DROP TABLE IF EXISTS Ksiazka;
GO

CREATE TABLE Ksiazka( 
    Ksiazka_ID INT IDENTITY, 
    ISBN VARCHAR(20), 
    Tytul VARCHAR(300), 
    Autor VARCHAR(200), 
    Rok_Wydania INT, 
    Cena DECIMAL(10,2), 
    Wypozyczona_Ostatni_Miesiac BIT, 
    CONSTRAINT Ksiazka_PK PRIMARY KEY (Ksiazka_ID), 
    CONSTRAINT Ksiazka_UK_ISBN UNIQUE (ISBN))
GO

CREATE TABLE Egzemplarz( 
    Egzemplarz_ID INT IDENTITY,
    Sygnatura CHAR(8),
    Ksiazka_ID INT,
    CONSTRAINT Egzemplarz_PK PRIMARY KEY (Egzemplarz_ID),
    CONSTRAINT Egzemplarz_UK_Sygnatura UNIQUE (Sygnatura),
    CONSTRAINT Egzemplarz_FK FOREIGN KEY (Ksiazka_ID) REFERENCES Ksiazka (Ksiazka_ID) ON DELETE CASCADE);
GO

CREATE TABLE Czytelnik(   
    Czytelnik_ID INT IDENTITY,
    PESEL CHAR(11),
    Nazwisko VARCHAR(30),
    Miasto VARCHAR(30),
    Data_Urodzenia DATE,
    Ostatnie_Wypozyczenie DATE,
    CONSTRAINT Czytelnik_PK PRIMARY KEY (Czytelnik_ID),
    CONSTRAINT Czytelnik_UK_PESEL UNIQUE (PESEL))
GO

CREATE TABLE Wypozyczenie(   
    Wypozyczenie_ID INT IDENTITY,
    Czytelnik_ID INT,
    Egzemplarz_ID INT,
    Data DATE,
    Liczba_Dni INT,
    CONSTRAINT Wypozyczenie_PK PRIMARY KEY (Wypozyczenie_ID),
    CONSTRAINT Wypozyczenie_FK_Czytelnik FOREIGN KEY (Czytelnik_ID) REFERENCES Czytelnik (Czytelnik_ID) ON DELETE CASCADE,
    CONSTRAINT Wypozyczenie_FK_Egzemplarz FOREIGN KEY (Egzemplarz_ID) REFERENCES Egzemplarz (Egzemplarz_ID) ON DELETE CASCADE)
GO

SET IDENTITY_INSERT Ksiazka ON
INSERT INTO Ksiazka (Ksiazka_ID,ISBN,Tytul,Autor,Rok_Wydania,Cena) VALUES
(1, '83-246-0279-8',     'Microsoft Access. Podrecznik administratora',                     'Helen Feddema',         2006, 69),
(2, '83-246-0653-X',     'SQL Server 2005. Programowanie. Od podstaw',                      'Robert Vieira',         2007, 97),
(3, '978-83-246-0549-1', 'SQL Server 2005. Wycisnij wszystko',                              'Eric L. Brown',         2007, 57),
(4, '978-83-246-1258-1', 'PHP, MySQL i MVC. Tworzenie witryn WWW opartych na bazie danych', 'Wlodzimierz Gajda',     2010, 79),
(5, '978-83-246-2060-9', 'Access 2007 PL. Seria praktyk',                                   'Andrew Unsworth',       2009, 39),
(6, '978-83-246-2188-0', 'Czysty kod. Podrecznik dobrego programisty',                      'Robert C. Martin',      2010, 67);
SET IDENTITY_INSERT Ksiazka OFF
GO

SET IDENTITY_INSERT Egzemplarz ON
INSERT INTO Egzemplarz (Egzemplarz_ID,Ksiazka_ID,Sygnatura) VALUES
(1,  5, 'S0001'),
(2,  5, 'S0002'),
(3,  1, 'S0003'),
(4,  1, 'S0004'),
(5,  1, 'S0005'),
(6,  2, 'S0006'),
(7,  3, 'S0007'),
(8,  3, 'S0008'),
(9,  3, 'S0009'),
(10, 1, 'S0010'),
(11, 2, 'S0011'),
(12, 3, 'S0012'),
(13, 1, 'S0013'),
(14, 2, 'S0014'),
(15, 5, 'S0015'),
(16, 6, 'S0016'),
(17, 1, 'S0017'),
(18, 1, 'S0018'),
(19, 2, 'S0019'),
(20, 3, 'S0020'),
(21, 4, 'S0021'),
(22, 5, 'S0022'),
(23, 5, 'S0023'),
(24, 4, 'S0024'),
(25, 5, 'S0025'),
(26, 6, 'S0026'),
(27, 5, 'S0027'),
(28, 5, 'S0028'),
(29, 5, 'S0029'),
(30, 5, 'S0030'),
(31, 5, 'S0031'),
(32, 1, 'S0032'),
(33, 2, 'S0033'),
(34, 2, 'S0034'),
(35, 2, 'S0035'),
(36, 2, 'S0036'),
(37, 2, 'S0037'),
(38, 2, 'S0038'),
(39, 3, 'S0039'),
(40, 1, 'S0040'),
(41, 2, 'S0041'),
(42, 3, 'S0042'),
(43, 4, 'S0043'),
(44, 5, 'S0044'),
(45, 6, 'S0045'),
(46, 1, 'S0046'),
(47, 1, 'S0047'),
(48, 2, 'S0048'),
(49, 3, 'S0049'),
(50, 3, 'S0050')
SET IDENTITY_INSERT Egzemplarz OFF
GO

SET IDENTITY_INSERT Czytelnik ON
INSERT INTO Czytelnik (CZYTELNIK_ID,PESEL,NAZWISKO,MIASTO,DATA_URODZENIA) VALUES
(1, '55101011111', 'Kowalski', 'Wroclaw',  '1955-10-10'),
(2, '60101033333', 'Maliniak', 'Wroclaw',  '1960-10-10'),
(3, '65120122222', 'Nowak',    'Warszawa', '1965-12-01'),
(4, '55010111111', 'Kozak',    'Swidnica', '1955-01-01'),
(5, '99020222222', 'Tester',   'Nigdzie',  '1999-02-02'),
(6, '98030333333', 'Nowacki',  'Halo',     '1998-03-03')
SET IDENTITY_INSERT Czytelnik OFF
GO

SET IDENTITY_INSERT Wypozyczenie ON
INSERT INTO Wypozyczenie (Wypozyczenie_ID,Czytelnik_ID,Egzemplarz_ID,Data,Liczba_Dni) VALUES
(1, 4, 26, '2019-7-11', 57),
(2, 3, 21, '2019-3-17', 87),
(3, 6, 6, '2019-1-20', 74),
(4, 4, 1, '2019-2-13', 9),
(5, 3, 17, '2019-11-24', 83),
(6, 3, 8, '2019-4-18', 84),
(7, 6, 1, '2019-6-16', 46),
(8, 1, 4, '2019-9-7', 10),
(9, 5, 46, '2019-1-3', 29),
(10, 6, 33, '2019-6-20', 73),
(11, 1, 2, '2019-7-13', 75),
(12, 1, 37, '2019-11-14', 19),
(13, 5, 16, '2019-3-23', 45),
(14, 6, 15, '2019-7-19', 64),
(15, 2, 9, '2019-7-8', 86),
(16, 2, 20, '2019-8-2', 84),
(17, 3, 20, '2019-3-1', 84),
(18, 4, 8, '2019-7-24', 66),
(19, 3, 9, '2019-10-24', 55),
(20, 4, 35, '2019-8-19', 3),
(21, 5, 19, '2019-2-21', 74),
(22, 5, 40, '2019-12-18', 40),
(23, 5, 29, '2019-3-28', 46),
(24, 2, 17, '2019-7-7', 70),
(25, 1, 41, '2019-12-11', 50),
(26, 3, 5, '2019-4-16', 55),
(27, 1, 13, '2019-3-28', 79),
(28, 4, 23, '2019-12-1', 39),
(29, 3, 23, '2019-6-22', 67),
(30, 2, 14, '2019-3-5', 18),
(31, 1, 6, '2019-5-19', 94),
(32, 5, 25, '2019-12-6', 23),
(33, 6, 48, '2019-3-11', 2),
(34, 5, 37, '2019-9-27', 19),
(35, 4, 34, '2019-1-11', 6),
(36, 1, 43, '2019-4-8', 2),
(37, 4, 7, '2019-10-17', 88),
(38, 4, 22, '2019-5-17', 96),
(39, 5, 3, '2019-9-6', 68),
(40, 1, 28, '2019-5-20', 21),
(41, 1, 20, '2019-8-23', 81),
(42, 3, 30, '2019-2-8', 54),
(43, 1, 39, '2019-1-20', 15),
(44, 6, 6, '2019-10-1', 80),
(45, 5, 14, '2019-10-22', 86),
(46, 4, 13, '2019-1-4', 79),
(47, 1, 36, '2019-11-7', 83),
(48, 5, 31, '2019-2-20', 95),
(49, 6, 1, '2019-1-16', 19),
(50, 6, 48, '2019-12-27', 85),
(51, 1, 40, '2019-3-15', 87),
(52, 2, 8, '2019-3-22', 92),
(53, 5, 39, '2019-8-28', 25),
(54, 5, 27, '2019-11-1', 63),
(55, 1, 50, '2019-8-16', 99),
(56, 2, 18, '2019-11-15', 70),
(57, 6, 41, '2019-1-23', 14),
(58, 3, 43, '2019-5-9', 71),
(59, 5, 2, '2019-3-28', 21),
(60, 1, 37, '2019-2-9', 28),
(61, 6, 11, '2019-12-24', 98),
(62, 5, 20, '2019-12-28', 37),
(63, 6, 23, '2019-11-28', 27),
(64, 3, 34, '2019-7-13', 43),
(65, 4, 27, '2019-1-23', 65),
(66, 2, 5, '2019-11-20', 82),
(67, 2, 30, '2019-6-28', 29),
(68, 2, 24, '2019-7-5', 5),
(69, 4, 21, '2019-9-28', 45),
(70, 2, 10, '2019-3-27', 84),
(71, 2, 2, '2019-6-11', 91),
(72, 5, 10, '2019-6-2', 87),
(73, 4, 29, '2019-2-21', 27),
(74, 5, 38, '2019-1-16', 96),
(75, 4, 21, '2019-10-24', 58),
(76, 4, 23, '2019-4-27', 85),
(77, 2, 35, '2019-5-20', 64),
(78, 4, 1, '2019-3-2', 88),
(79, 6, 23, '2019-10-17', 22),
(80, 2, 4, '2019-12-18', 27),
(81, 1, 34, '2019-8-11', 57),
(82, 1, 46, '2019-10-24', 99),
(83, 3, 50, '2019-9-3', 35),
(84, 2, 42, '2019-7-10', 58),
(85, 6, 29, '2019-9-9', 15),
(86, 2, 44, '2019-7-8', 18),
(87, 2, 43, '2019-6-11', 77),
(88, 6, 45, '2019-8-9', 29),
(89, 4, 37, '2019-6-10', 32),
(90, 5, 45, '2019-7-15', 5),
(91, 1, 49, '2019-10-20', 17),
(92, 3, 32, '2019-10-12', 30),
(93, 1, 33, '2019-6-9', 59),
(94, 5, 27, '2019-12-8', 30),
(95, 1, 7, '2019-9-26', 31),
(96, 2, 22, '2019-2-8', 64),
(97, 1, 43, '2019-4-4', 88),
(98, 2, 16, '2019-2-23', 21),
(99, 1, 36, '2019-2-18', 32),
(100, 5, 11, '2019-4-3', 34)
SET IDENTITY_INSERT Wypozyczenie OFF
GO

-- Testy --
-- Mozna sprawdzic czasy przez Query -> Include Client Statistics (shift + alt + s)
SET STATISTICS TIME ON

SELECT DISTINCT c.PESEL, c.Nazwisko FROM Egzemplarz e
JOIN Ksiazka k		ON e.Ksiazka_ID    = k.Ksiazka_ID
JOIN Wypozyczenie w ON e.Egzemplarz_ID = w.Egzemplarz_ID
JOIN Czytelnik c	ON c.Czytelnik_ID  = w.Czytelnik_ID
-- Tworzymy tabelkê z niepowtarzalnych peseli i nazwisk, do³¹czamy do tego inne tabelki na podstawie powtarzaj¹cych siê kluczy.
-- Client processing time:		1.2
-- Total execution time:		1.6
-- Wait time on server replies:	0.4

SELECT c.PESEL, c.Nazwisko
FROM Czytelnik c WHERE c.Czytelnik_ID IN
(SELECT w.Czytelnik_ID FROM Wypozyczenie w
JOIN Egzemplarz e ON e.Egzemplarz_ID = w.Egzemplarz_ID
JOIN Ksiazka k	  ON e.Ksiazka_ID    = k.Ksiazka_ID)
-- Tworzymy tabelkê z peseli i nazwisk, gdzie ID czytelnika pokrywa siê z uzyskanymi w podzapytaniu ID czytelników wypo¿yczaj¹cych ksi¹¿ki.
-- Client processing time:		0.3
-- Total execution time:		1.0
-- Wait time on server replies:	0.7

-- inne zapytanie 
SELECT c.PESEL, c.Nazwisko
FROM Czytelnik c WHERE c.Czytelnik_ID IN
(SELECT w.Czytelnik_ID FROM Wypozyczenie w, Egzemplarz e, Ksiazka k WHERE e.Egzemplarz_ID = w.Egzemplarz_ID AND e.Ksiazka_ID = k.Ksiazka_ID)

-- Client processing time:		0.3
-- Total execution time:		0.4
-- Wait time on server replies:	0.1

SET STATISTICS TIME OFF

-- SET SHOWPLAN_ALL ON / OFF -- jakies dodatkowe info