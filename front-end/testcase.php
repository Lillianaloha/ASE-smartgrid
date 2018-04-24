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
$myfile = fopen("portnumber.txt", "r");
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
    echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($server_sock)) . "\n";
}
do 
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
    
//    $generate_IP = '160.39.232.239';
    
//    Andrew's virtual machine
    $generate_IP = '160.39.136.200';
    
//    $generate_IP = '160.39.232.82';
    $generate_Port = 8001;
    
//    $time_rate = 10;
//    $sampling_rate = 2;
    
    $time_rate = "0010";
    $sampling_rate = "0002";
    
//To get the data from user interface
//    if( $_GET["time"] || $_GET["rate"] ) {
//      $time_rate = "00".$_GET['time'];
//      $sampling_rate = "00".$_GET['rate'];
//   }  
    
    //Or we can pre-set IP and port?
    
    do 
    {
        /*
        if (false === ($buf = socket_read($msgsock, 2048, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        */
        
        /*
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        if (false === ($buf = socket_read($generate_Port, 4, PHP_NORMAL_READ))) 
        {
            echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        */
        
        //Get both time and sampling rate from client
        /*
        if (false === ($buf = socket_read($time_rate, 4, PHP_NORMAL_READ))) 
        {
            echo "Failed to read time rate: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        echo "time rate is :$time_rate";
        
        if (false === ($buf = socket_read($sampling_rate, 4, PHP_NORMAL_READ))) 
        {
            echo "Failed to read sampling rate: " . socket_strerror(socket_last_error($msgsock)) . "\n";
            break 2;
        }
        */
        
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
        while(true)
        {
            //Read from generator
            if (false === ($buf = socket_read($generator_socket, 200)))
            {
                echo "Failed to get data from generator..." . socket_strerror(socket_last_error($msgsock)) . "\n";
                break 2;
            }
            if ($buf == '')
            {
                echo "No more data to read\n";
                break;
            }
            echo $buf;
            $content = str_replace($txt,strval($port+1),$txt);
            $myfile2 = fopen("portnumber.txt", "w");
            fwrite($myfile2,$content);
            
            if (!$buf = trim($buf))
            {
                continue;
            }
//            echo $buf;
        }
        echo "Exited while loop\n";
        
        /*
        if (!$buf = trim($buf)) 
        {
            continue;
        }
        if ($buf == 'quit') 
        {
            break;
        }
        if ($buf == 'shutdown') 
        {
            socket_close($msgsock);
            break 2;
        }
        $talkback = "PHP: You said '$buf'.\n";
        socket_write($msgsock, $talkback, strlen($talkback));
        echo "$buf\n";
        */
    }
    while (true); 
    echo "Reading Smart Grid Generator session complete\n";
    socket_close($client_sock);
}
while (true);
socket_close($server_sock);
?>
