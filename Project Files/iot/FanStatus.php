<?php
include_once 'DataBaseCon.inc';


//show errors
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

$conn= @mysqli_connect($host, $user, $pwd, $sql_db);

if (!$conn) {
	      echo "<p>Database Connection Failure</p>";
	    } 
	    else 
	    {
				$query = "SELECT FanOnOrOff FROM FanStatus ORDER BY id DESC LIMIT 1";        
				$result = mysqli_query($conn, $query); 
            while ($row = mysqli_fetch_assoc($result))
            {
					echo $row["FanOnOrOff"];  
            }
       }
       mysqli_close($conn);

?>



            
            

            