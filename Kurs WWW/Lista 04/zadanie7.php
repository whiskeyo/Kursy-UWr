<?php
session_name("SESJA3");
//session_id( "96de90f89b630dbafcd32891673ca748ss" );
session_start();
session_regenerate_id();
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta http-equiv="X-UA-Compatible" content="ie=edge">
        <title>Lista 4 - Zadanie 7</title>
    </head>

    <body>
        <h3>Funkcja do sesji</h3>
        <?php
            echo "ID:\t".session_id()."<br>\n";
            echo "Name:\t".session_name()."<br>\n";
            echo "Path:\t".session_save_path()."<br>\n";
            if (!isset($_SESSION["var"])) 
                $_SESSION["var"] = 0;
            else 
                $_SESSION["var"]++;
            if (isset($_SESSION["var"])) 
                echo ( $_SESSION["var"] )."<br>\n";
        ?>

        <h3>Ciastko sesyjne</h3>
        <?php
            echo $_COOKIE["SESJA3"];
        ?>
    </body>
</html>
