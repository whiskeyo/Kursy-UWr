-- zadanie 1 --
SELECT DISTINCT City
FROM [SalesLT].[Address] JOIN [SalesLT].[SalesOrderHeader]
ON [SalesLT].[SalesOrderHeader].ShipToAddressID = [SalesLT].[Address].AddressID
WHERE ShipDate <= '2020-03-08'
ORDER BY City ASC

-- to jest v1 niedzialajace --
SELECT DISTINCT ShipToAddressID
FROM [SalesLT].[SalesOrderHeader]
WHERE ShipDate <= '2020-03-08'
ORDER BY ShipToAddressID ASC

-- zadanie 2 --
SELECT Name, COUNT(Name) AS Counter
FROM [SalesLT].[ProductModel]
GROUP BY Name
--HAVING COUNT(Name) > 1 -- ta linijka byłaby okej, gdyby były powtarzające się produkty

-- zadanie 3--
SELECT 
    adresy.City, 
    COUNT(kl_adresy.CustomerID) AS "Liczba klientow",
    COUNT(DISTINCT klienci.SalesPerson) AS "Liczba obslugujacych"
FROM 
    [SalesLT].[Address] AS adresy, 
    [SalesLT].[Customer] AS klienci,
    [SalesLT].[CustomerAddress] AS kl_adresy
WHERE
    adresy.AddressID = kl_adresy.AddressID 
    AND klienci.CustomerID = kl_adresy.CustomerID
GROUP BY adresy.City

-- zadanie 4--

-- zadanie 5--
SELECT 
    kl.FirstName AS Imie,
    kl.LastName AS Nazwisko,
    SUM(sp_detail.UnitPriceDiscount) AS Zaoszczedzono
FROM
    [SalesLT].[Customer] AS kl,
    [SalesLT].[SalesOrderDetail] AS sp_detail,
    [SalesLT].[SalesOrderHeader] AS sp_header
WHERE
    kl.CustomerID = sp_header.CustomerID
    AND sp_detail.SalesOrderID = sp_header.SalesOrderID
GROUP BY kl.FirstName, kl.LastName
ORDER BY Zaoszczedzono DESC

-- zadanie 6--

-- zadanie 7--
ALTER TABLE [SalesLT].[Customer]
ADD CreditCardNumber VARCHAR(20) NOT NULL DEFAULT '0000-0000-0000-0000'