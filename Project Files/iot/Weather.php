<?php
    // show any errors
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ALL);
    $json = file_get_contents("http://api.openweathermap.org/data/2.5/weather?q=Melbourne,au&units=metric&APPID=fda2109659faa71995bdd900e8a6674a");
    $data = json_decode($json);
    $currentTemp = round($data->main->temp);
    
    echo "<p>Current Outside Temperature: " . $currentTemp . "°C</p>";
    echo "<div class='progress'>";
    echo "    <div class='progress-bar bg-info' style='width: " . $currentTemp * 2 ."%' role='progressbar' aria-valuenow='0' aria-valuemin='0' aria-valuemax='50'></div>";
    echo "</div>";
?>
