<!DOCTYPE html>
<html>
   <head>

   </head>
   <body>
        <?php 
            echo "POST: <br />";
            foreach ($_POST as $key => $value)
                echo $key.'='.$value.'<br />';
            
            echo "<br /><br />GET: <br />";
            foreach ($_GET as $key => $value)
                echo $key.'='.$value.'<br />'; 

            echo "<br /><br />REQUEST: <br />";
            foreach ($_REQUEST as $key => $value)
                echo $key.'='.$value.'<br />';
            
            echo "<br /><br />SERVER: <br />";
            while (list($var,$value) = each ($_SERVER)) 
                echo "$var => $value <br />";

        ?>
   </body>
</html>