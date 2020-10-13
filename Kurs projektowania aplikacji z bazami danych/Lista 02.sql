-- ZADANIE 1 --
-- procedura:
DROP PROCEDURE IF EXISTS exercise1proc
GO

CREATE PROCEDURE exercise1proc @days INT AS
BEGIN
	SELECT 
		czyt.PESEL, COUNT(wyp.Wypozyczenie_ID) AS LiczbaEgzemplarzy
	FROM	
		[master].[dbo].Czytelnik AS czyt,
		[master].[dbo].Wypozyczenie AS wyp
	WHERE
		czyt.Czytelnik_ID = wyp.Czytelnik_ID AND
		wyp.Liczba_Dni > @days
	GROUP BY czyt.PESEL
END
GO

EXEC exercise1proc @days = 13
GO

-- funkcja:
DROP FUNCTION IF EXISTS exercise1func
GO

CREATE FUNCTION exercise1func(@days int) RETURNS TABLE
RETURN
	SELECT 
	czyt.PESEL, COUNT(wyp.Wypozyczenie_ID) AS LiczbaEgzemplarzy
	FROM	
		[dbo].Czytelnik AS czyt,
		[dbo].Wypozyczenie AS wyp
	WHERE
		czyt.Czytelnik_ID = wyp.Czytelnik_ID AND
		wyp.Liczba_Dni > @days
	GROUP BY czyt.PESEL
GO

SELECT * FROM exercise1func(13)

-- ZADANIE 2 --
DROP TABLE IF EXISTS imiona
GO

CREATE TABLE imiona(id INT PRIMARY KEY, imie CHAR(20))
GO

INSERT INTO imiona VALUES(1, 'tomasz')
INSERT INTO imiona VALUES(2, 'bartosz')
INSERT INTO imiona VALUES(3, 'kacper')
INSERT INTO imiona VALUES(4, 'mateusz')
INSERT INTO imiona VALUES(5, 'lukasz')
INSERT INTO imiona VALUES(6, 'sebastian')
GO

DROP TABLE IF EXISTS nazwiska
GO

CREATE TABLE nazwiska(id INT PRIMARY KEY, nazwisko CHAR(20))
GO

INSERT INTO nazwiska VALUES(1, 'nowak')
INSERT INTO nazwiska VALUES(2, 'nowacki')
INSERT INTO nazwiska VALUES(3, 'kowal')
INSERT INTO nazwiska VALUES(4, 'kowalski')
INSERT INTO nazwiska VALUES(5, 'mlotek')
INSERT INTO nazwiska VALUES(6, 'mlotkowski')
INSERT INTO nazwiska VALUES(7, 'plotek')
GO

DROP PROCEDURE IF EXISTS exercise2proc
GO

CREATE PROCEDURE exercise2proc @n INT AS
BEGIN
	DROP TABLE IF EXISTS dane
	CREATE TABLE dane(imie CHAR(20), nazwisko CHAR(20))
	
	DECLARE @iterator INT
	SET @iterator = 1

	DECLARE @polowa		INT
	DECLARE @ileimion	INT
	DECLARE @ilenazwisk INT
	SET @ileimion   = (SELECT COUNT(id) FROM master.dbo.imiona)
	SET @ilenazwisk = (SELECT COUNT(id) FROM master.dbo.nazwiska)
	SET @polowa     = @ileimion * @ilenazwisk / 2

	IF (@n > @polowa)
		BEGIN
			;THROW 50001, 'n jest wieksze od polowy wszystkich mozliwosci', 1;
		END

	WHILE (@iterator <= @n)
	BEGIN
		DECLARE @imie	  CHAR(20)
		DECLARE @nazwisko CHAR(20)
		SET @imie	  = (SELECT TOP 1 imie FROM imiona ORDER BY NEWID())
		SET @nazwisko = (SELECT TOP 1 nazwisko FROM nazwiska ORDER BY NEWID())

		IF NOT EXISTS (SELECT * FROM dane WHERE imie = @imie AND nazwisko = @nazwisko)
		BEGIN
			INSERT INTO dane VALUES(@imie, @nazwisko)
			SET @iterator = @iterator + 1
		END
	END

	SELECT * FROM dane
END
GO

EXEC exercise2proc @n = 6
GO

-- ZADANIE 3 --
DROP PROCEDURE IF EXISTS exercise3proc
GO

DROP TYPE IF EXISTS Czytelnicy
GO

CREATE TYPE Czytelnicy AS TABLE(czytelnik_id INT)
GO

CREATE PROCEDURE exercise3proc @czytelnicy Czytelnicy READONLY AS
BEGIN
	SELECT 
		input.czytelnik_id, SUM(wyp.Liczba_Dni) AS suma_dni
	FROM	
		@czytelnicy AS input,
		[master].[dbo].Czytelnik AS czyt,
		[master].[dbo].Wypozyczenie AS wyp
	WHERE
		czyt.Czytelnik_ID = wyp.Czytelnik_ID AND
		czyt.Czytelnik_ID = input.czytelnik_id
	GROUP BY 
		input.czytelnik_id
END
GO

DECLARE @id_czyt Czytelnicy
INSERT INTO @id_czyt VALUES (1)
INSERT INTO @id_czyt VALUES (2)

EXEC exercise3proc @id_czyt
GO

-- Zadanie 4 -- (dynamic SQL) niestety nie wyszlo
DECLARE @tytul   VARCHAR(300)
DECLARE @autor   VARCHAR(200)
DECLARE @rok	 INT
DECLARE @wynik	 INT
DECLARE @request NVARCHAR(500)
DECLARE @params	 NVARCHAR(500)

SET @request = N'
	SELECT 
		COUNT(*) AS liczba_egzemplarzy
	FROM
		Ksiazka
	WHERE 
		Ksiazka.Tytul = @tytul
		AND Ksiazka.Autor = @autor
		AND Ksiazka.Rok_Wydania = @rok
'

SET @params = N'@tytul VARCHAR(300), @autor VARCHAR(200), @rok INT OUTPUT @wynik INT'

EXEC sp_executesql @request, @params, @wynik OUTPUT, @rok = 2007
GO

--------

DROP PROCEDURE IF EXISTS test4
GO


CREATE PROCEDURE test4 @tytulArg nvarchar(100) = NULL, @autorArg nvarchar(100) = NULL, @rokArg int = NULL AS
BEGIN
	DECLARE @SQLString nvarchar(500); 
	SET @SQLString =  
     N'SELECT * FROM Ksiazka  
     WHERE 
		Ksiazka.Tytul = @tytul AND
		Ksiazka.Autor = @autor AND
		Ksiazka.Rok_Wydania = @rok';
		
	DECLARE @ParmDefinition nvarchar(500);
	SET @ParmDefinition = N'@tytul nvarchar(100), @autor nvarchar(100), @rok nvarchar(100)';  
	EXECUTE sp_executesql @SQLString, @ParmDefinition, 
                      @tytul = @tytulArg, @autor = @autorArg, @rok = @rokArg;  
END
GO

EXEC test4 N'Microsoft Access. Podr?cznik administratora', N'Helen Feddema', 2006
GO