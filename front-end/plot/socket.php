<?php include "../db.php";?>

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

// Automatically change the port number for multiple connection tests
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
    
//    Zilin's virtual machine     
//    $generate_IP = '160.39.232.239';
//    $generate_IP = '160.39.232.82';   
        
//    Andrew's virtual machine   
    $generate_IP = '160.39.136.200';
    $generate_Port = 8001;
//    pre-set IP and port?     
//    $time_rate = 10;
//    $sampling_rate = 2;
    
//  To get the data from user interface
  if( $_GET["time"] || $_GET["rate"] ) {
      $time_rate = intval($_GET['time']);
      $sampling_rate = intval($_GET['rate']);
   }  
    // Make sure it successfully connects to the generator
    $generator_socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

    if ($generator_socket === false) 
    {
        echo "Failed to create generator socket: reason: " . socket_strerror(socket_last_error()) . "\n";
    }

    if((socket_connect($generator_socket,$generate_IP, $generate_Port)) == false)
    {
          echo "Failed to connect to generator.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
    }

    // Make sure the input numbers are valid
    if ($time_rate > 0)
        socket_write($generator_socket, $time_rate, 4);
    else
        echo "Invalid time input!\n";

    if ($sampling_rate > 0)
        socket_write($generator_socket, $sampling_rate, 4);
    else
        echo "Invalid sampling rate input!\n";
    
    // Set the number for each row in csv file for better graphing
    $j = 1;
    $total = "";
    static $total;
    while($buf = socket_read($generator_socket, 200))
    {
        //  Read from generator
//        echo $buf;
        $total .= $buf;
      
        $content = str_replace($txt,strval($port+1),$txt);
        $myfile2 = fopen("port.txt", "w");
        fwrite($myfile2,$content);
//        
    }
//    echo $total;
    // Redirect to the another page for graph
    if($total){
        header('Location: index_graph.php');
    }   
    // Create an empty file
    $file_r = fopen('file.csv', 'w');
    fclose($file_r);
    
//    global $conn;
    $buf_array = explode('{', $total);
    for ($i = 1; $i < count($buf_array); $i++){       
        $str = str_replace(array('}'), '', $buf_array[$i]);
        $array = array(explode(',',strval($j).','.$str));
        
        // To parse the string so as to save in mysql table
        list($Va,$Vb,$Vc,$Ia,$Ib,$Ic,$Total_Power,$Total_Fundamental_Power,$Phase_A_Power,$Phase_B_Power,$Phase_C_Power,$Reactive_Power,$Phase_A_Reactive_Power,$Phase_B_Reactive_Power,$Phase_C_Reactive_Power,$Consumed_Power,$Sold_Power) = explode(",",$str);
       
        $query = "INSERT INTO smartgrid2(Va,Vb,Vc,Ia,Ib,Ic,`Total Power`,`Total Fundamental Power`,`Phase A Power`,`Phase B Power`,`Phase C Power`,`Reactive Power`,`Phase A Reactive Power`,`Phase B Reactive Power`,`Phase C Reactive Power`,`Consumed Power`,`Sold Power`) VALUES ('$Va','$Vb','$Vc','$Ia','$Ib','$Ic','$Total_Power','$Total_Fundamental_Power','$Phase_A_Power','$Phase_B_Power','$Phase_C_Power','$Reactive_Power','$Phase_A_Reactive_Power','$Phase_B_Reactive_Power','$Phase_C_Reactive_Power','$Consumed_Power','$Sold_Power')";
        
        $result = mysqli_query($conn, $query);
        
        
        // To save it in an csv file so as to graph
        if(!$result) {
            die('Query FAILED' . mysqli_error());
        }
        
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
    
    // Close the socket
    socket_close($client_sock);
    break;
}
socket_close($server_sock);  
?>
