<?php
    if(isset($_GET["session"])){
        session_id($_GET["session"]);
        session_start();
        if(!isset($_SESSION["amount"])){
            $_SESSION["amount"]=rand(0,100);
        }
    }
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta http-equiv="X-UA-Compatible" content="ie=edge">
        <title>Lista 4 - Zadanie 8 - attack SID</title>
    </head>
    <body>
        <form action="zadanie8-sid.php" method="get">
            ID SESJI: <input type="text" name="session">
        <input type="submit" value="Wyślij">
        </form>

        <?php
            if(isset($_SESSION["amount"]))
                echo $_SESSION["amount"];
        ?>

    </body>
</html>