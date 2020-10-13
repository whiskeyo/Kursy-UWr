<?php
    if ($_SERVER["REQUEST_METHOD"] == "POST") 
        {
        $name = $_POST["name"];
        $surname = $_POST["surname"];
        $email = $_POST["email"];
        $card = $_POST["card"];
        $date = $_POST["date"];
        $cvc = $_POST["cvc"];
        $amount = $_POST["amount"];

        function checkName($name)
        {
            return !is_numeric($name) && !empty($name);
        }

        function checkEmail($email)
        {
            return filter_var($email, FILTER_VALIDATE_EMAIL);
        }

        function checkCard($card)
        {
            return preg_match('/[0-9]{4}-[0-9]{4}-[0-9]{4}-[0-9]{4}\b/', $card);
        }

        function checkExpiryDate($date)
        {
            return preg_match('/[0-1][1-9]\/[0-9]{2}\b/', $date);
        }

        function checkCvc($cvc)
        {
            return is_numeric($cvc);
        }

        function checkAmount($amount)
        {
            return is_numeric($amount) && ($amount > 0);
        }
    }
?>
<!DOCTYPE html>
<html lang="pl">
    <head>
        <title>Lista 4 - zadanie 1</title>
        <meta charset="UTF-8">
        <style>
            td,th,body { font-family:Verdana, Arial, Helvetica, sans-serif; font-size:10pt; }
            label { display:block; margin-bottom: 20px; }
        </style>
    </head>
    <body>
        <h3>Personal data</h3>
        <form name="dane" action="zadanie1.php" method="post">
            <label for="card">
                Card number:
                <input type="text" name="card" id="card" value=<?php if(isset($card)): echo $card; endif;?>>
                <?php if(isset($card) && !checkCard($card)): echo '<div>Incorrect card number (XXXX-XXXX-XXXX-XXXX)</div>'; endif;?>
            </label>

            <label for="date">
                Expiry date:
                <input type="text" name="date" id="date" value=<?php if(isset($date)): echo $date; endif;?>>
                <?php if(isset($date) && !checkExpiryDate($date)): echo '<div>Incorrect expiry date (XX/XX)</div>'; endif;?>
            </label>

            <label for="cvc">
                CVC:
                <input type="text" name="cvc" id="cvc" value=<?php if(isset($cvc)): echo $cvc; endif;?>>
                <?php if(isset($cvc) && !checkCvc($cvc)): echo '<div>Incorrect CVC code (XXX)</div>'; endif;?>
            </label>

            <label for="amount">
                Amount:
                <input type="text" name="amount" id="amount" value=<?php if(isset($amount)): echo $amount; endif;?>>
                <?php if(isset($amount) && !checkAmount($amount)): echo '<div>Incorrect amount</div>'; endif;?>
            </label>

            <label for="name">
                Name:
                <input type="text" name="name" id="name" value=<?php if(isset($name)): echo $name; endif;?>>
                <?php if(isset($name) && !checkName($name)): echo '<div>Incorrect name</div>'; endif;?>
            </label>

            <label for="surname">
                Surname:
                <input type="text" name="surname" id="surname" value=<?php if(isset($surname)): echo $surname; endif;?>>
                <?php if(isset($surname) && !checkName($surname)): echo '<div>Incorrect surname</div>'; endif;?>
            </label>

            <label for="email">
                E-mail:
                <input type="text" name="email" id="email" value=<?php if(isset($email)): echo $email; endif;?>>
                <?php if(isset($email) && !checkEmail($email)): echo '<div>Incorrect email address</div>'; endif;?>
            </label>
            <input type="submit" value="Wyślij">
        </form>
    </body>
</html>