<?php

//header('Location: plot/index_graph.html') 
    
//    
//$time_rate = $_GET['time'];
//$sampling_rate = "000".$_GET['rate'];
//echo $time_rate;
//echo $sampling_rate;
    
//    $a = "10";
//$c = intval($a);
//if($c == 10){echo $c;}
//

$myfile = fopen("portnumber.txt", "r");
$txt = fread($myfile,filesize("portnumber.txt"));
$port = intval($txt);
echo $port;
$content = str_replace($txt,strval($port+1),$txt);
$myfile2 = fopen("portnumber.txt", "w");
fwrite($myfile2,$content);

    
    
    
    
?>