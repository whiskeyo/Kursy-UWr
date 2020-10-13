<?php
	session_id($_GET["session"]);
	session_start();
	if(isset($_POST["kwota"])){
		$_SESSION["kwota"]=$_POST["kwota"];
	}
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta http-equiv="X-UA-Compatible" content="ie=edge">
		<title>Lista 4 - Zadanie 8 - simple attack</title>
	</head>
	<body>
		<form action="<?php echo "zadanie8-simple.php?session=".$_GET["session"] ?>" method="post">
			KWOTA: <input type="text" name="kwota">
		<input type="submit" value="Wyślij">
		</form>

		<?php
				echo $_SESSION["kwota"];
		?>
	</body>
</html>