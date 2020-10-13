-- Zadanie 1 --
DROP TABLE IF EXISTS Towary
GO

CREATE TABLE Towary(ID INT PRIMARY KEY, NazwaTowaru VARCHAR(50))
GO

INSERT INTO Towary VALUES(1, 'snickers')
INSERT INTO Towary VALUES(2, 'bounty')
INSERT INTO Towary VALUES(3, 'mars')
GO

DROP TABLE IF EXISTS Kursy
GO

CREATE TABLE Kursy(Waluta VARCHAR(3) PRIMARY KEY, CenaPLN MONEY)
GO

INSERT INTO Kursy VALUES('PLN', 1.00)
INSERT INTO Kursy VALUES('EUR', 4.56)
INSERT INTO Kursy VALUES('NOK', 0.39)
INSERT INTO Kursy VALUES('KWD', 13.20)
INSERT INTO Kursy VALUES('JPY', 0.038)
GO

DROP TABLE IF EXISTS Ceny
GO

CREATE TABLE Ceny(TowarID INT REFERENCES Towary(ID), Waluta VARCHAR(3) REFERENCES Kursy(Waluta), Cena MONEY)
GO
-- jen japonski nie jest obslugiwany, wiec usuwamy ten rekord
-- aktualne wpisy nalezy nadpisac nowa waluta albo stworzyc nowe
-- 1 -> PLN, JPY, EUR, NOK
-- 2 -> PLN, KWD
-- 3 -> PLN, NOK
INSERT INTO Ceny VALUES(1, 'PLN', 2.49)
INSERT INTO Ceny VALUES(2, 'PLN', 3.19)
INSERT INTO Ceny VALUES(3, 'PLN', 1.79)
INSERT INTO Ceny VALUES(1, 'JPY', 78.27)
INSERT INTO Ceny VALUES(1, 'EUR', 1.55)
INSERT INTO Ceny VALUES(1, 'NOK', 10.09)
INSERT INTO Ceny VALUES(2, 'KWD', 0.99)
INSERT INTO Ceny VALUES(3, 'NOK', 4.39)
GO 

DECLARE z1towary CURSOR FOR SELECT TowarID, Waluta, Cena FROM Ceny ORDER BY TowarID
DECLARE @Ttowarid INT, @Twaluta VARCHAR(3), @Tcena MONEY
OPEN z1towary
FETCH NEXT FROM z1towary INTO @Ttowarid, @Twaluta, @Tcena
WHILE (@@FETCH_STATUS = 0)
BEGIN
	-- PRINT 'O: ID: ' + CAST(@Ttowarid AS CHAR(3)) + ' Waluta: ' + CAST(@Twaluta AS CHAR(3)) + ' Cena: ' + CAST(@Tcena AS CHAR(5))
	DECLARE z1waluty CURSOR FOR SELECT Waluta, CenaPLN FROM Kursy
	DECLARE @Wwaluta VARCHAR(3), @Wcenapln MONEY
	OPEN z1waluty
	FETCH NEXT FROM z1waluty INTO @Wwaluta, @Wcenapln
	WHILE (@@FETCH_STATUS = 0)
	BEGIN
		-- PRINT 'I: Waluta: ' + CAST(@Wwaluta AS CHAR(3)) + ' Kurs: ' + CAST(@Wcenapln AS CHAR(5))
		IF (@Twaluta = @Wwaluta) -- ceny w pln zostaja niezmienione
		BEGIN
			SET @Tcena = @Tcena / @Wcenapln
			UPDATE Ceny SET Cena = @Tcena WHERE TowarID = @Ttowarid AND Waluta = @Twaluta
			--DELETE FROM Ceny WHERE TowarID = @Ttowarid AND @Twaluta = @Wwaluta
			--INSERT INTO Ceny VALUES(@Ttowarid, @Twaluta, @Tcena)
		END
		FETCH NEXT FROM z1waluty INTO @Wwaluta, @Wcenapln
	END
	CLOSE z1waluty
	DEALLOCATE z1waluty
	FETCH NEXT FROM z1towary INTO @Ttowarid, @Twaluta, @Tcena
END
CLOSE z1towary
DEALLOCATE z1towary
GO

SELECT * FROM Ceny ORDER BY TowarID, Waluta DESC
GO

-- lekko poprawione zadanie 1 --
ALTER TABLE Ceny NOCHECK CONSTRAINT ALL
DELETE from Kursy WHERE Waluta = 'JPY'
ALTER TABLE Ceny CHECK CONSTRAINT ALL
GO

DECLARE z1towary CURSOR FOR SELECT TowarID, Waluta, Cena FROM Ceny ORDER BY TowarID
DECLARE z1waluty CURSOR FOR SELECT Waluta, CenaPLN FROM Kursy

DECLARE @Ttowarid INT, @Twaluta VARCHAR(3), @Tcena MONEY
DECLARE @Curr_towarid INT, @Curr_cenaPLN MONEY
DECLARE @Wwaluta VARCHAR(3), @Wkurs MONEY
DECLARE @delete BIT

OPEN z1towary
FETCH NEXT FROM z1towary INTO @Ttowarid, @Twaluta, @Tcena
SET @Curr_towarid = @Ttowarid
SET @Curr_cenaPLN = (SELECT Cena from Ceny WHERE TowarID = @Curr_towarid AND Waluta = 'PLN')
WHILE (@@FETCH_STATUS = 0)
BEGIN
	-- PRINT 'O: ID: ' + CAST(@Ttowarid AS CHAR(3)) + ' Waluta: ' + CAST(@Twaluta AS CHAR(3)) + ' Cena: ' + CAST(@Tcena AS CHAR(5))
	OPEN z1waluty
	FETCH NEXT FROM z1waluty INTO @Wwaluta, @Wkurs
	SET @delete = 1
	WHILE (@@FETCH_STATUS = 0)
	BEGIN
		IF (@Twaluta = @Wwaluta)
		BEGIN
			SET @Tcena = @Curr_cenaPLN / @Wkurs
			UPDATE Ceny SET Cena = @Tcena WHERE TowarID = @Ttowarid AND Waluta = @Twaluta
			SET @delete = 0
		END
		FETCH NEXT FROM z1waluty INTO @Wwaluta, @Wkurs
	END
	IF @delete=1
		DELETE FROM Ceny WHERE TowarID = @Ttowarid AND Waluta = @Twaluta
	CLOSE z1waluty
	FETCH NEXT FROM z1towary INTO @Ttowarid, @Twaluta, @Tcena
	IF @Curr_towarid != @Ttowarid
	BEGIN
		SET @Curr_towarid = @Ttowarid
		SET @Curr_cenaPLN = (SELECT Cena from Ceny WHERE TowarID = @Curr_towarid AND Waluta = 'PLN')
	END
END
CLOSE z1towary
DEALLOCATE z1waluty
DEALLOCATE z1towary
GO

SELECT * FROM Ceny ORDER BY TowarID, Waluta DESC
GO

-- Zadanie 2 -- NIEDOKONCZONE
DROP TABLE IF EXISTS Employees
GO

CREATE TABLE Employees(ID INT PRIMARY KEY, SalaryGros MONEY)
GO

INSERT INTO Employees VALUES (1, 3450)
INSERT INTO Employees VALUES (2, 11230)
GO

DROP TABLE IF EXISTS SalaryHistory
GO

CREATE TABLE SalaryHistory(
	ID			INT PRIMARY KEY,
	EmployeeID	INT,
	Year		INT,
	Month		INT,
	SalaryNet	MONEY,
	SalaryGros	MONEY)
GO

-- Zadanie 3 --
DROP TABLE IF EXISTS Bufor
DROP TABLE IF EXISTS Historia
DROP TABLE IF EXISTS Parametry
GO

CREATE TABLE Bufor(ID INT IDENTITY PRIMARY KEY, AdresUrl VARCHAR(200), OstatnieWejscie DATETIME)
CREATE TABLE Historia(ID INT IDENTITY PRIMARY KEY, AdresUrl VARCHAR(200), OstatnieWejscie DATETIME)
CREATE TABLE Parametry(nazwa VARCHAR(200), wartosc INT)
GO

INSERT INTO Parametry VALUES('tiny', 5)
GO

-- ten wyzwalacz dziala niepoprawnie przy wstawianiu kilku wartosci jednym insertem, tzn.
-- INSERT INTO Bufor VALUES(..., ...), (..., ...), (..., ...)...
CREATE TRIGGER przenies_do_historii ON Bufor INSTEAD OF INSERT
AS
BEGIN
	DECLARE @I_AdresUrl varchar(200), @I_OstatnieWejscie DATETIME;
	SELECT @I_AdresUrl=AdresUrl, @I_OstatnieWejscie=OstatnieWejscie FROM INSERTED;
	IF EXISTS (SELECT 1 FROM Bufor WHERE AdresUrl=@I_AdresUrl)
		UPDATE Bufor SET OstatnieWejscie=@I_OstatnieWejscie WHERE AdresUrl=@I_AdresUrl;
	ELSE
	BEGIN
		DECLARE @Rows INT,  @MaxRows INT;
		SET @Rows = (SELECT COUNT(*) FROM Bufor);
		SET @MaxRows = (SELECT TOP 1 wartosc FROM Parametry);
		IF (@Rows < @MaxRows)
			INSERT INTO Bufor SELECT AdresUrl, OstatnieWejscie FROM INSERTED
		ELSE
		BEGIN
			DECLARE @B_ID INT, @B_AdresUrl varchar(200), @B_OstatnieWejscie DATETIME
			SELECT TOP 1 @B_ID=ID, @B_AdresUrl=AdresUrl, @B_OstatnieWejscie=OstatnieWejscie FROM Bufor ORDER BY OstatnieWejscie
			IF EXISTS (SELECT 1 FROM Historia WHERE AdresUrl=@B_AdresUrl)
				UPDATE Historia SET OstatnieWejscie=@B_OstatnieWejscie WHERE AdresUrl=@B_AdresUrl
			ELSE
				INSERT INTO Historia VALUES(@B_AdresUrl, @B_OstatnieWejscie)
			DELETE FROM Bufor WHERE ID=@B_ID
			INSERT INTO Bufor SELECT AdresUrl, OstatnieWejscie FROM INSERTED
		END
	END
END	
GO

INSERT INTO Bufor VALUES('uchoroszczy.pl',		'01/04/2012 11:30:00')
INSERT INTO Bufor VALUES('uchoroszczy.pl',		'01/04/2012 11:30:01')
INSERT INTO Bufor VALUES('PZW.pl',				'01/04/2012 11:30')
INSERT INTO Bufor VALUES('AgresywnaMasa.pl',	'01/04/2012 11:30')
INSERT INTO Bufor VALUES('Allegro.pl',			'01/04/2012 11:30')
INSERT INTO Bufor VALUES('zabka.pl',			'01/04/2012 10:30')
INSERT INTO Bufor VALUES('KoronaPoradnik.pl',	'01/04/2012 10:30')
INSERT INTO Bufor VALUES('YouTube.com',			'01/04/1999 11:30')
INSERT INTO Bufor VALUES('Biedronka.pl',		'01/04/2012 10:30') -- Wyrzuci YouTube.com do historii
INSERT INTO Bufor VALUES('testowastrona.pl',	'01/04/2012 11:31')
INSERT INTO Bufor VALUES('rampampam.pl',		'01/04/2012 09:15')
INSERT INTO Bufor VALUES('iostatnianateraz.pl',	'01/04/2012 11:32')

SELECT * FROM Bufor
SELECT * FROM Historia
GO



