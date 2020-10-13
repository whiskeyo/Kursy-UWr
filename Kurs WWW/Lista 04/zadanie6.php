<?php
    setcookie( "sernik" , "z czekoladą", time() + 3600, "/" );
    session_start();
    $_SESSION["sesja"]=rand(0 ,100);
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta http-equiv="X-UA-Compatible" content="ie=edge">
        <title>Lista 4 - Zadanie 6</title>
    </head>
    <body>
        <h3>Wartość ciastka</h3>

        Ciastko: <?php echo $_COOKIE["sernik"]; ?>    <br>
        Ciastko sesyjne: <?php echo $_SESSION["sesja"]; ?>

    </body>
</html>