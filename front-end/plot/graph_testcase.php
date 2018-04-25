<?php
error_reporting(E_ALL);
/* Allow the script to hang around waiting for connections. */
set_time_limit(0);
/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();
//Main Computer data that holds SQL Database and website
$address = 'localhost';
//$port = 8273;
$myfile = fopen("port.txt", "r");
$txt = fread($myfile,filesize("port.txt"));
$port = intval($txt);
// Creating Server Socket
if (($server_sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}
if (socket_bind($server_sock, $address, $port) === false) {
    echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}
if (socket_listen($server_sock, 5) === false) {
    echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($server_sock)) . "\n";
}

while(true)
{
    if (($client_sock = socket_accept($server_sock)) === false) 
    {
        echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($client_sock)) . "\n";
        break;
    }
    /* Send instructions. */
    $msg = "\nWelcome to the PHP Test Server. \n" .
        "To quit, type 'quit'. To shut down the server type 'shutdown'.\n";
    socket_write($client_sock, $msg, strlen($msg));
    //  We can pre-set IP and port?    
//    $generate_IP = '160.39.232.239';
    
//    Andrew's virtual machine
    $generate_IP = '160.39.136.200';
    
//    $generate_IP = '160.39.232.82';
    $generate_Port = 8001;
    
//    $time_rate = 10;
//    $sampling_rate = 2;
    
    $time_rate = "0010";
    $sampling_rate = "0002";
    
//  To get the data from user interface
//  if( $_GET["time"] || $_GET["rate"] ) {
//      $time_rate = "00".$_GET['time'];
//      $sampling_rate = "000".$_GET['rate'];
//   }  

    $generator_socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

    if ($generator_socket === false) 
    {
        echo "Failed to create generator socket: reason: " . socket_strerror(socket_last_error()) . "\n";
    }

    if((socket_connect($generator_socket,$generate_IP, $generate_Port)) == false)
    {
          echo "Failed to connect to generator.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
    }


    if ($time_rate > 0)
        socket_write($generator_socket, $time_rate, 4);
    else
        echo "Invalid time input!\n";

    if ($sampling_rate > 0)
        socket_write($generator_socket, $sampling_rate, 4);
    else
        echo "Invalid sampling rate input!\n";

    //Read input from generator
    $j = 1;
    $total = "";
    static $total;
    while($buf = socket_read($generator_socket, 200))
    {
        //Read from generator
//        echo $buf;
        $total .= $buf;
      
        $content = str_replace($txt,strval($port+1),$txt);
        $myfile2 = fopen("port.txt", "w");
        fwrite($myfile2,$content);
//        
    }
//    echo $total;
    header('Location: index_graph.php');
    $buf_array = explode('{', $total);
    
    $file_rewrite = fopen('file.csv', 'w');
    fclose($file_rewrite);
    
    for ($i = 1; $i < count($buf_array); $i++){       
        $str = str_replace(array('}'), '', $buf_array[$i]);
        $array = array(explode(',',strval($j).','.$str));
        $fp = fopen('file.csv', 'a');
        foreach ($array as $fields) {
            fputcsv($fp, array_slice($fields,0,4));
        }
        $j = $j + 1;
    }
//    echo "hi";
//    header('Location: index_graph.php');
//    echo "<br>"."Exited while loop";
//    echo "<br>"."Reading Smart Grid Generator session complete";
    socket_close($client_sock);
    break;
}
socket_close($server_sock);  
?>