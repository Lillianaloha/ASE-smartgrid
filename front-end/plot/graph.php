<?php include "../db.php"?>
<?php
error_reporting(E_ALL);
/* Allow the script to hang around waiting for connections. */
set_time_limit(0);
/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

//Main Computer data that holds SQL Database and website
$address = 'localhost';
//$port = 8235;
$myfile = fopen("port.txt", "r");
$txt = fread($myfile,filesize("portnumber.txt"));
$port = intval($txt);

// Creating Server Socket
if (($server_sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}
if (socket_bind($server_sock, $address, $port) === false) {
    echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}
if (socket_listen($server_sock, 5) === false) {
    echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}
while(true) 
{
    if (($client_sock = socket_accept($server_sock)) === false) 
    {
        echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
        break;
    }
    
    /* Instructions on the terminal. */
    $msg = "\nWelcome to the PHP Test Server. \n" .
        "You are connected.\n";
    socket_write($client_sock, $msg, strlen($msg));
    
//    $generate_IP = '160.39.232.239';
//    $generate_IP = '160.39.136.12';
    $generate_IP = '160.39.136.200';
    $generate_Port = 8001;
    
//    $time_rate = 10;
//    $sampling_rate = 1;
    $time_rate = '0010';
    $sampling_rate = '0005';       
//    input something here
//    if( $_GET["time"] || $_GET["rate"] ) {
//      $time_rate = intval($_GET['time']);
//      $sampling_rate = intval($_GET['rate']);
//   }  
    
    //  To get the data from user interface
//  if( $_GET["time"] || $_GET["rate"] ) {
//      $time_rate = "00".$_GET['time'];
//      $sampling_rate = "000".$_GET['rate'];
//   } 
////    
    //Or we can pre-set IP and port?
    
    //Create Socket and connect to Generator
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

    //Now wait until transmission completes...
    $j = 1;
    while($buf = socket_read($generator_socket, 200))
    {
        //Read from generator
        echo $buf;
        $content = str_replace($txt,strval($port+1),$txt);
        $myfile2 = fopen("port.txt", "w");
        fwrite($myfile2,$content);
//            $buf = strval($counter).','.$buf;
        $buf_array = explode('{', $buf);

        for ($i = 1; $i < count($buf_array) - 1; $i++){
            $str = str_replace(array('}'), '', $buf_array[$i]);
            $array = array(explode(',',strval($j).','.$str));

//            database
            $str = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17";
            global $conn;
            list($Id, $Va,$Vb,$Vc,$Ia,$Ib,$Ic,$Total_Power,$Total_Fundamental_Power,$Phase_A_Power,$Phase_B_Power,$Phase_C_Power,$Reactive_Power,$Phase_A_Reactive_Power,$Phase_B_Reactive_Power,$Phase_C_Reactive_Power,$Consumed_Power,$Sold_Power) = explode(",",strval($j).','.$str);
            $query = "INSERT INTO smartgrid VALUES ('$Id','$Va','$Vb','$Vc','$Ia','$Ib','$Ic','$Total_Power','$Total_Fundamental_Power','$Phase_A_Power','$Phase_B_Power','$Phase_C_Power','$Reactive_Power','$Phase_A_Reactive_Power','$Phase_B_Reactive_Power','$Phase_C_Reactive_Power','$Consumed_Power','$Sold_Power')";
//                $result = mysqli_query($conn, $query);
//                if(!$result) {
//                    die('Query FAILED' . mysqli_error());
//                }
//                echo "1 record added";

//                csv
            $filename = strval($port).'.csv';
            $fp = fopen($filename, 'w');
            foreach ($array as $fields) {
                fputcsv($fp, array_slice($fields,0,4));
            }
            $j = $j + 1;
        }
//        $j = $j + 1;
        header('Location: index_graph.php');
        break;
    }
//    echo "Reading Smart Grid Generator session complete\n";
    socket_close($client_sock);
}
socket_close($server_sock);
?>